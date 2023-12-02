// Fill out your copyright notice in the Description page of Project Settings.


#include "ASPlayerCharacter.h"
#include "ASEnemy.h"
#include "ASBulletHitIInterface.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/ASWeapon.h"
#include "Item/ASGrenade.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AASPlayerCharacter::AASPlayerCharacter() :
	bIsAiming(false),
	CameraDefaultFOV(60.f),
	CameraZoomedFOV(40.f),
	CurrentCameraFOV(0.f),
	ZoomSpeed(5.f),
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	HipTurnRate(90.f),
	HipLookUpRate(90.f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.0f),
	CrossHairSpreadMultiplier(1.25f),
	ShootTimeDuration(0.05f),
	bFiringBullet(false),
	AttackDelayTime(0.1)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.f;
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러를 기준으로 회전
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 50.f);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCam"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false; // 스프링 암 기준으로 회전 x

	//

	GetCharacterMovement()->bOrientRotationToMovement = false; //입력한 방향으로 로테이션
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	bUseControllerRotationYaw = true;

	defaultMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;

	CombatState = ECombatState::ECS_Unoccupied;

	//------------------------------------------------------------------------------Cross Spread Init
	//생성자에서 초기화 하려니까 너무 길어져서.. 이쪽으로 옮김 
	SpreadMin = 0;
	SpreadHipMax = 0.3f;
	SpreadAimingMax = 0.3f;
	SpreadCurrent = 0.f;
	SpreadDecreaseSpeed = 0.25f;
	SpreadIncreaseSpeed = 0.01f;
	
	//------------------------------------------------------------------------------Skill
	TraceDistance = 300;

	//-----------------------------------------------------------------------------Hand 
	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Hand Component"));
}

// Called when the game starts or when spawned
void AASPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(SpawnDefaultWeapon());
	
	Camera->FieldOfView = CameraDefaultFOV;
	CurrentCameraFOV = CameraDefaultFOV;
}

void AASPlayerCharacter::PossessedBy(AController* NewController)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(NewController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(defaultMappingContext, 0);
		}
	}
}

// Called every frame
void AASPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraInterpZoom(DeltaTime);
	SetLookRates();
	CalcCrossHairSpread(DeltaTime);
	CalcAimingSpeed();
	//SKill
	BuildTypeSkillTrace();

	FHitResult ItemTraceResult;
	FVector HitLocation;
	TraceUnderCrosshairs(ItemTraceResult, HitLocation);

	if(ItemTraceResult.bBlockingHit)
	{
		//ItemTrace 
	}

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("%d"),EquippedWeapon->GetCurrentAmmoCount()));
}

// Called to bind functionality to input
void AASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &AASPlayerCharacter::CharacterMove);

		//Look
		EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &AASPlayerCharacter::CharacterLook);

		//Fire
		EnhancedInputComponent->BindAction(IAAttack, ETriggerEvent::Triggered, this, &AASPlayerCharacter::MouseLeftClick);

		//Test Skill
		EnhancedInputComponent->BindAction(IASkill1, ETriggerEvent::Triggered, this, &AASPlayerCharacter::UseSkill);
		EnhancedInputComponent->BindAction(IASkill2, ETriggerEvent::Triggered, this, &AASPlayerCharacter::UseGrenadeSkill);

		//Aiming
		EnhancedInputComponent->BindAction(IAAiming,ETriggerEvent::Triggered,this,&AASPlayerCharacter::AimingButtonPressed);
		EnhancedInputComponent->BindAction(IAAiming,ETriggerEvent::Completed,this,&AASPlayerCharacter::AimingButtonReleased);

		//Reloading
		EnhancedInputComponent->BindAction(IAReloading,ETriggerEvent::Triggered,this,&AASPlayerCharacter::Reloading);
	}
}


void AASPlayerCharacter::CharacterMove(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, movementVector.Y);
		AddMovementInput(RightDirection, movementVector.X);
	}
}

void AASPlayerCharacter::CharacterLook(const FInputActionValue& value)
{
	FVector2D LookAxisVector = value.Get<FVector2D>() ;

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		AddControllerPitchInput(LookAxisVector.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AASPlayerCharacter::MouseLeftClick()
{
	if (CombatState == ECombatState::ECS_Reloading) return;

	if(CombatState == ECombatState::ECS_UsedSkill)
	{
		CreateBarrier();
		CombatState = ECombatState::ECS_Unoccupied;
	}
	else
	{
		FireWeapon();
	}
}

void AASPlayerCharacter::CalcAimingSpeed()
{
	//if(bIsAiming)
	//	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	//else
	//	GetCharacterMovement()->MaxWalkSpeed = defaultMovementSpeed;
}

void AASPlayerCharacter::EquipWeapon(AASWeapon* WeaponToEquip)
{
	if(WeaponToEquip)
	{
		//WeaponToEquip->GetAreaSphere()->SetCollisionResponseToAllChannels(
		//	ECollisionResponse::ECR_Ignore);
		//WeaponToEquip->GetCollisionBox()->SetCollisionResponseToAllChannels(
		//	ECollisionResponse::ECR_Ignore);
		
		
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName("RightHandSocket");
		if(HandSocket)
		{
			HandSocket->AttachActor(Cast<AActor>(WeaponToEquip),GetMesh());
		}

		EquippedWeapon = WeaponToEquip;
	}
}

void AASPlayerCharacter::DropWeapon()
{
	if(EquippedWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld,true);
		EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);
	}
}

AASWeapon* AASPlayerCharacter::SpawnDefaultWeapon()
{
	AASWeapon* spawnWeapon = NewObject<AASWeapon>();
	//spawnWeapon->CreateWeapon(1001);

	AASWeapon* returnWeapon = GetWorld()->SpawnActor<AASWeapon>(spawnWeapon->StaticClass());
	returnWeapon->CreateWeapon(1001);
	if(returnWeapon)
		return returnWeapon;
	return nullptr;

	//GetWorld()->SpawnActor<AASWeapon>(DefaultWeapon);
	//if(DefaultWeapon)
	//{
	//	//AASWeapon* sapwnWeapon = NewObject<AASWeapon>();
	//	//sapwnWeapon->CreateWeapon(1001);
	//	//AASWeapon* spawnWeapon = GetWorld()->SpawnActor<AASWeapon>(DefaultWeapon);
	//
	//	return spawnWeapon;
	//}
	//return nullptr;
}

void AASPlayerCharacter::Reloading()
{
	UAnimMontage* reloading = Montages[MontageType::Reloading];
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && reloading)
	{
		AnimInstance->Montage_Play(reloading);
		AnimInstance->Montage_JumpToSection(FName("StartReloading"));
		CombatState = ECombatState::ECS_Reloading;
	}
}

bool AASPlayerCharacter::WeaponHasAmmo()
{
	if(EquippedWeapon == nullptr) return false;

	return EquippedWeapon->GetCurrentAmmoCount() > 0;
}

void AASPlayerCharacter::GrapClip()
{
	if(EquippedWeapon == nullptr) return;
	if (HandSceneComponent == nullptr) return;

	int32 ClipBoneIndex{ EquippedWeapon->GetItemMesh()->GetBoneIndex(EquippedWeapon->GetClipBoneName())};
	ClipTransform = EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	HandSceneComponent->AttachToComponent(GetMesh(), AttachmentRules,FName("Hand_L"));
	HandSceneComponent->SetWorldTransform(ClipTransform);

	EquippedWeapon->SetMovingClip(true);
}

void AASPlayerCharacter::ReplaceClip()
{
	EquippedWeapon->SetMovingClip(false);
	CombatState = ECombatState::ECS_Unoccupied;
}

void AASPlayerCharacter::FireWeapon()
{
	if(bFiringBullet) return;
	if(!WeaponHasAmmo()) return;
	//const USkeletalMeshSocket* BarrelSocket = EquippedWeapon->ItemMesh->GetSocketByName("BarrelSocket");
	if (EquippedWeapon)
	{
		//const FTransform muzzleTr = BarrelSocket->GetSocketTransform(GetMesh());
		
		//if (MuzzleFlash)
		//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, muzzleTr);

		FHitResult BeamHitResult;
		bool bIsBeam = GetBeamEndLocation(EquippedWeapon->GetBarrelSocketTransForm().GetLocation(), BeamHitResult);
		if(bIsBeam)
		{
			// Does hit Actor implement BulletHitInterface?
			if (BeamHitResult.GetActor()) {
				DrawDebugPoint(GetWorld(), BeamHitResult.ImpactPoint, 3, FColor::Red, false, 10, 0);
				IASBulletHitIInterface* BulletHitInterface = Cast<IASBulletHitIInterface>(BeamHitResult.GetActor());

				if (BulletHitInterface) 
				{
					BulletHitInterface->BulletHit_Implementation(BeamHitResult);
				}

				AASEnemy* HitEnemy = Cast<AASEnemy>(BeamHitResult.GetActor());
				if (HitEnemy)
				{
					if (BeamHitResult.BoneName.ToString() == HitEnemy->GetHeadBone())
					{
						//HeadShot
						UGameplayStatics::ApplyDamage(
							BeamHitResult.GetActor(),
							EquippedWeapon->GetHeadDamage(),
							GetController(),
							this,
							UDamageType::StaticClass());
						//UE_LOG(LogTemp, Warning, TEXT("Hit Componenet: %s"), *BeamHitResult.BoneName.ToString());
					}
					else
					{
						UGameplayStatics::ApplyDamage(
							BeamHitResult.GetActor(),
							EquippedWeapon->GetDamage(),
							GetController(),
							this,
							UDamageType::StaticClass());
					}
				}
			} 
			else 
			{
				// Spawn default particle
				if (ImpactParticle) {
					UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(),
						ImpactParticle,
						BeamHitResult.Location);
				}
				DrawDebugPoint(GetWorld(), BeamHitResult.ImpactPoint, 3, FColor::Red, false, 10, 0);
			}

			EquippedWeapon->ShowShotParticles(BeamHitResult);
			//if (ShotLineParticle)
			//{
			//	UParticleSystemComponent* beam = UGameplayStatics::SpawnEmitterAtLocation(
			//		GetWorld(),
			//		ShotLineParticle,
			//		muzzleTr);
			//		
			//	if (beam)
			//	{
			//		beam->SetVectorParameter(FName("Target"), BeamHitResult.Location);
			//	}
			//}
		}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montages[MontageType::Attack])
	{
		AnimInstance->Montage_Play(Montages[MontageType::Attack]);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}

	if(EquippedWeapon)
		EquippedWeapon->DecrementAmmo();

	StartCrossHairBulletFire();
}

void AASPlayerCharacter::StartCrossHairBulletFire()
{
	bFiringBullet = true;
	
	GetWorldTimerManager().SetTimer(CrossHairShootTimer,this,
		&AASPlayerCharacter::FinishCrossHairBulletFire,ShootTimeDuration);

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,"Fire");
}

void AASPlayerCharacter::FinishCrossHairBulletFire()
{
	bFiringBullet = false;
	GetWorldTimerManager().ClearTimer(CrossHairShootTimer);
}

void AASPlayerCharacter::CreateBarrier()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform ft;

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,1,FColor::Red,FString::Printf(TEXT("%f : %f :%f"),GroundPlacementPoint.X,GroundPlacementPoint.Y,GroundPlacementPoint.Z));
	}
	
	ft.SetLocation(GroundPlacementPoint);
	GetWorld()->SpawnActor<AActor>(barrierActor, ft, spawnParams);
}

void AASPlayerCharacter::UseGrenadeSkill()
{
	if (grenadeActor && EquippedWeapon)
	{
		FRotator MuzzleRotation = GetActorRotation();
		FVector MuzzleLocation = EquippedWeapon->GetBarrelSocketTransForm().GetLocation() + MuzzleRotation.Vector() * 100.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			AASGrenade* Grenade = GetWorld()->SpawnActor<AASGrenade>(grenadeActor, MuzzleLocation, MuzzleRotation);
		}
	}
}

void AASPlayerCharacter::UseSkill()
{
	CombatState = ECombatState::ECS_UsedSkill;
}

void AASPlayerCharacter::BuildTypeSkillTrace()
{
	if(CombatState != ECombatState::ECS_UsedSkill) return;
	
	FVector ground = (FVector::UpVector * -100.f); //지면까지의 거리
	
	FVector Start = GetActorLocation() + (GetActorForwardVector() * TraceDistance) + ground;
	FVector End = Start + FVector::UpVector * 1000;
	FVector HalfSize =  FVector(5,5,5);
	
	TArray<AActor*> IgnoreActors;
	FHitResult Hit;
	
	UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start,
		End,
		HalfSize,
		FRotator::ZeroRotator,
		TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForOneFrame,
		Hit,true
		);


	if(Hit.bBlockingHit)
	{
		GroundPlacementPoint = Hit.ImpactPoint;
	}
}

void AASPlayerCharacter::AimingButtonPressed()
{
	bIsAiming = true;
}

void AASPlayerCharacter::AimingButtonReleased()
{
	bIsAiming = false;
}

void AASPlayerCharacter::CameraInterpZoom(float deltaTime)
{
	if(bIsAiming)
	{
		CurrentCameraFOV = FMath::FInterpTo(CurrentCameraFOV,CameraZoomedFOV,deltaTime,ZoomSpeed);
		Camera->SetFieldOfView(CurrentCameraFOV);
	}
	else
	{	
		CurrentCameraFOV =  FMath::FInterpTo(CurrentCameraFOV,CameraDefaultFOV,deltaTime,ZoomSpeed);
		Camera->SetFieldOfView(CurrentCameraFOV);
	}
}

void AASPlayerCharacter::SetLookRates()
{
	if(bIsAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void AASPlayerCharacter::CalcCrossHairSpread(float deltaTime)
{
	//속도에 따라 크로스헤드 벌어지는 정도
	//ex) 현재 속도 0 ~ 600 : 0 ~ 1
	FVector2D walkSpeed = FVector2D(0.f,600.f);
	FVector2D velocityMultiply = FVector2D{0.0f,1.0f};
	FVector velocity = GetVelocity();
	velocity.Z = 0;
	
	if(bFiringBullet)
	{
		IncreaseSpread(SpreadIncreaseSpeed);
	}
	
	if(SpreadMin <= SpreadCurrent)
	{
		DecreaseSpread(deltaTime * SpreadDecreaseSpeed);
	}

	CrossHairShootingFactor = SpreadCurrent;

	CrossHairVelocityFactor = FMath::GetMappedRangeValueClamped(walkSpeed,velocityMultiply,velocity.Size());
	CrossHairSpreadMultiplier = 0.5f + CrossHairVelocityFactor + CrossHairShootingFactor - CrossHairAimFactor;
}

void AASPlayerCharacter::IncreaseSpread(float increaseAmount)
{
	float max  = 0;
	bIsAiming ? max = SpreadAimingMax : max = SpreadHipMax;
	if(SpreadCurrent >= max)
	{
		SpreadCurrent = max;
	}
	else
	{
		SpreadCurrent += increaseAmount;
	}
}

void AASPlayerCharacter::DecreaseSpread(float decreaseAmount)
{
	if(SpreadCurrent <= SpreadMin)
	{
		SpreadCurrent = SpreadMin;
	}
	else
	{
		SpreadCurrent -= decreaseAmount;
	}
}

bool AASPlayerCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult)
{
	FVector OutBeamLocation;

	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation);

	if(bCrosshairHit)
	{
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else
	{
		
	}

	const FVector WeaponTraceStart = MuzzleSocketLocation;
	const FVector StartToEnd {OutBeamLocation - MuzzleSocketLocation};
	const FVector WeaponTraceEnd = MuzzleSocketLocation + StartToEnd * 1.25f;

	GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		WeaponTraceStart,
		WeaponTraceEnd,
		ECollisionChannel::ECC_Visibility);

	if (OutHitResult.bBlockingHit)
	{
		OutHitResult.Location = OutBeamLocation;
		return true;
	}
	return false;
}

bool AASPlayerCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	// Get viewport size 
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrossHairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrossHairLocation, CrossHairWorldPosition, CrossHairWorldDirection);

	if(bScreenToWorld)
	{
		//
		const FVector Start{CrossHairWorldPosition};
		const FVector End{Start + CrossHairWorldDirection * 50000.f};
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility);

		if(OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

