// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASGrenade.h"
#include "Character/ASEnemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/ASEnemy.h"
#include <Kismet/GameplayStatics.h>

AASGrenade::AASGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(CollisionComp);
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this,&AASGrenade::OnHitActor);
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	OnHitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OverLap Comp"));
	OnHitCollision->OnComponentBeginOverlap.AddDynamic(this, &AASGrenade::OnBeginOverlap);
	OnHitCollision->SetupAttachment(CollisionComp);
	OnHitCollision->InitSphereRadius(100.f);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	explosionEffectScale = FVector(5,5,5);

	Damage = 100;
	
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AASGrenade::Explosion, countDownTime);
}

void AASGrenade::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&AASGrenade::Explosion,countDownTime);
}

void AASGrenade::Explosion()
{
	
}

void AASGrenade::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AASEnemy* HitEnemy = Cast<AASEnemy>(OtherActor);
	if (HitEnemy)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			UGameplayStatics::GetPlayerController(GetWorld(),0),
			this,
			UDamageType::StaticClass());
		HitEnemy->BulletHit_Implementation(SweepResult);
		HitEnemy->ShowHitNumber(Damage, this->GetActorLocation(), false);
	}
}
void AASGrenade::OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Grenade Hit"));
	}
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles,GetActorLocation(),GetActorRotation(), explosionEffectScale);
		if (!Explosion)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn particle system component."));
			return;
		}

		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

		if(OtherComp->IsA<AASEnemy>())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}
		Destroy();
	}

}
