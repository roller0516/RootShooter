// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASEnemy.h"
#include "Character/ASPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "ASEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameCore/ASGame/ASGameInstance.h"
#include "Data/ASItemPrimaryData.h"
#include "Item/ASItemBase.h"
#include "Item/ASWeapon.h"
// Sets default values
AASEnemy::AASEnemy() :
	Health(100.f),
	MaxHealth(100.f),
	HealthBarDisplayTime(4.f),
	bCanHitReact(true),
	HitReactTimeMin(.5f),
	HitReactTimeMax(3.f),
	HitNumberDestroyTime(1.5f),
	bStunned(false),
	StunChance(0.5f),
	AttackRFast(TEXT("AttackRFast")),
	AttackLFast(TEXT("AttackLFast")),
	AttackR(TEXT("AttackR")),
	AttackL(TEXT("AttackL")),
	LeftWeaponSocket(TEXT("FX_Trail_L_03")),
	RightWeaponSocket(TEXT("FX_Trail_R_03")),
	bDying(false),
	bCanAttack(true),
	AttackWaitTime(1.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());

	CombatRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombarRange"));
	CombatRangeSphere->SetupAttachment(GetRootComponent());

	LeftWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWeaponBone"));
	LeftWeaponCollision->SetupAttachment(GetMesh(), FName("LeftWeaponBone"));
	RightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWeaponBone"));
	RightWeaponCollision->SetupAttachment(GetMesh(), FName("RightWeaponBone"));
}

// Called when the game starts or when spawned
void AASEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AASEnemy::AgroSphereOverlap);

	CombatRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AASEnemy::CombatRangeOverlap);

	CombatRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AASEnemy::CombatRangeEndOverlap);

	LeftWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AASEnemy::OnLeftWeaponOverlap);
	RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AASEnemy::OnRightWeaponOverlap);

	LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	EnemyController = Cast<AASEnemyController>(GetController());

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("CanAttack"), true);
	}

	const FVector WorldPatrolPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoint);
	const FVector WorldPatrolPoint2 = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoint2);

	DrawDebugSphere(GetWorld(), WorldPatrolPoint, 25.f, 12, FColor::Red, true);
	DrawDebugSphere(GetWorld(), WorldPatrolPoint2, 25.f, 12, FColor::Red, true);

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPoint"), WorldPatrolPoint);
		EnemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPoint2"), WorldPatrolPoint2);

		EnemyController->RunBehaviorTree(BehaviorTree);
	}
}

void AASEnemy::ShowHealthBar_Implementation()
{
	GetWorldTimerManager().ClearTimer(HealthBarTimer);
	GetWorldTimerManager().SetTimer(
		HealthBarTimer, 
		this, 
		&AASEnemy::HideHealthBar, 
		HealthBarDisplayTime);
}

void AASEnemy::Die()
{
	if (bDying) return;
	bDying = true;

	HideHealthBar();
	DropItem();
	//GetMesh()->SetCollisionProfileName("Ragdoll", true);
	//GetMesh()->SetSimulatePhysics(true);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}

	if (DeathParticles) {

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticles, GetActorTransform().GetLocation(), FRotator(0.f), true);
	}

	//if (EnemyController)
	//{
	//	EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	//	EnemyController->StopMovement();
	//}

	Destroy();
}

void AASEnemy::PlayHitMontage(FName Section, float PlayRate)
{
	if (bCanHitReact)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance)
		{
			AnimInstance->Montage_Play(HitMontage, PlayRate);
			AnimInstance->Montage_JumpToSection(Section, HitMontage);
		}

		bCanHitReact = false;
		const float HitReactTime{ FMath::FRandRange(HitReactTimeMin, HitReactTimeMax) };
		GetWorldTimerManager().SetTimer(
			HitReactTimer, 
			this, 
			&AASEnemy::ResetHitReactTimer, 
			HitReactTime
		);
	}
}

void AASEnemy::ResetHitReactTimer()
{
	bCanHitReact = true;
}

void AASEnemy::StoreHitNumber(UUserWidget* HitNumber, FVector Location)
{
	HitNumbers.Add(HitNumber, Location);

	FTimerHandle HitNumberTimer;
	FTimerDelegate HitNumberDelegate;
	HitNumberDelegate.BindUFunction(this, FName("DestroyHitNumber"), HitNumber);
	GetWorld()->GetTimerManager().SetTimer(
		HitNumberTimer, 
		HitNumberDelegate, 
		HitNumberDestroyTime, 
		false);
}

void AASEnemy::DestroyHitNumber(UUserWidget* HitNumber)
{
	HitNumbers.Remove(HitNumber);
	HitNumber->RemoveFromParent();
}

void AASEnemy::UpdateHitNumbers()
{
	for (auto& HitPair : HitNumbers)
	{
		UUserWidget* HitNumber{ HitPair.Key };
		const FVector Location{ HitPair.Value };
		FVector2D ScreenPosition;
		UGameplayStatics::ProjectWorldToScreen(
			GetWorld()->GetFirstPlayerController(), 
			Location, 
			ScreenPosition);
		HitNumber->SetPositionInViewport(ScreenPosition);
	}
}

void AASEnemy::AgroSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	auto playerCharacter = Cast<AASPlayerCharacter>(OtherActor);
	if (playerCharacter)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), playerCharacter);
	}
}

void AASEnemy::SetStunned(bool Stunned)
{
	bStunned = Stunned;

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("Stunned"), Stunned);
	}
}

void AASEnemy::CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	auto playerCharacter = Cast<AASPlayerCharacter>(OtherActor);
	if (playerCharacter)
	{
		bInAttackRange = true;
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(
				TEXT("InAttackRange"),
				true
			);
		}
	}
}

void AASEnemy::CombatRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;
	auto playerCharacter = Cast<AASPlayerCharacter>(OtherActor);
	if (playerCharacter)
	{
		bInAttackRange = false;
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(
				TEXT("InAttackRange"),
				false
			);
		}
	}
}

void AASEnemy::PlayAttackMontage(FName Section, float PlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(Section, AttackMontage);
	}

	bCanAttack = false;
	GetWorldTimerManager().SetTimer(
		AttackWaitTimer,
		this,
		&AASEnemy::ResetCanAttack,
		AttackWaitTime
	);

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("CanAttack"), true);
	}
}

FName AASEnemy::GetAttackSectionName()
{
	FName SectionName;
	const int32 Section{ FMath::RandRange(1, 4) };
	switch (Section)
	{
	case 1:
		SectionName = AttackRFast;
		break;
	case 2:
		SectionName = AttackLFast;
		break;
	case 3:
		SectionName = AttackR;
		break;
	case 4:
		SectionName = AttackL;
		break;
	}
	return SectionName;
}

void AASEnemy::OnLeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AASPlayerCharacter>(OtherActor);
	if (Character)
	{
		DoDamage(Character);
		SpawnBlood(Character, LeftWeaponSocket);
	}
}

void AASEnemy::OnRightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AASPlayerCharacter>(OtherActor);
	if (Character)
	{
		DoDamage(Character);
		SpawnBlood(Character, RightWeaponSocket);
	}
}

void AASEnemy::ActivateLeftWeapon()
{
	LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AASEnemy::DeactivateLeftWeapon()
{
	LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AASEnemy::ActivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AASEnemy::DeactivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AASEnemy::DoDamage(AASPlayerCharacter* Victim)
{
	if (Victim == nullptr) return;

	FRandomStream RandomStream(FDateTime::Now().GetTicks());
	int32 RandomDamage = RandomStream.RandRange(MinDamage, MaxDamage);

	UGameplayStatics::ApplyDamage(
		Victim,
		RandomDamage,
		EnemyController,
		this,
		UDamageType::StaticClass()
	);
}

void AASEnemy::SpawnBlood(AASPlayerCharacter* Victim, FName socketName)
{
	const USkeletalMeshSocket* TipSocket{ GetMesh()->GetSocketByName(socketName) };
	if (TipSocket)
	{
		const FTransform SocketTransform{ TipSocket->GetSocketTransform(GetMesh()) };
		if (Victim->GetBloodParticles())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Victim->GetBloodParticles(), SocketTransform);
		}
	}
}

void AASEnemy::FinishDeath()
{
	Destroy();
}

void AASEnemy::ResetCanAttack()
{
	bCanAttack = true;
}

void AASEnemy::DropItem()
{
	UASGameInstance* gi = Cast<UASGameInstance>(GetGameInstance());
	TArray<FItemBaseData*> data = gi->ItemData->GetRandomItemDatas();
	for(int i = 0 ; i < data.Num(); i++)
	{
		FTransform tr;
		
		tr.SetLocation(GetActorLocation() + FVector(0, 0, FMath::RandRange(10, 20)));
		tr.SetRotation(GetActorRotation().Quaternion());

		if(gi->ItemData->GetItemData(data[i]->ItemID))
		{
			AASItemBase* item = GetWorld()->SpawnActor<AASItemBase>(AASItemBase::StaticClass(), tr);
			item->SetActorScale3D(FVector(0.5f,0.5f,0.5f));
			item->CreateItem(data[i]->ItemID);
			item->SetItemProperties(E_ItemState::E_Drop);
		}
		else if(gi->ItemData->GetWeaponData(data[i]->ItemID))
		{
			AASWeapon* item = GetWorld()->SpawnActor<AASWeapon>(AASWeapon::StaticClass(), tr);
			if(item)
			{
				item->SetActorScale3D(FVector(1,1,1));
				item->CreateItem(data[i]->ItemID);
				item->SetItemProperties(E_ItemState::E_Drop);
			}
		}
		else
		{
			
		}

		
	}
}

// Called every frame
void AASEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHitNumbers();
}

// Called to bind functionality to input
void AASEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AASEnemy::BulletHit_Implementation(FHitResult HitResult) {

	if (ImpactSound) {

		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (ImpactParticles) {

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, HitResult.Location, FRotator(0.f), true);
	}
	ShowHealthBar();

	const float Stunned = FMath::FRandRange(0.f, 1.f);
	if (Stunned <= StunChance)
	{
		PlayHitMontage(FName("HitReactFront"));
		SetStunned(true);
	}

	if (GEngine) {
		//GEngine->AddOnScreenDebugMessage(-1,1,FColor::Blue,FString::Printf(TEXT("hit")));
	}
}

float AASEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EnemyController)
	{
		auto Character = Cast<AASPlayerCharacter>(DamageCauser);
		if (Character)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsObject(FName("Target"), Character);
		}
	}

	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}
