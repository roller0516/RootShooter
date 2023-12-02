// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ASGrenade.generated.h"

UCLASS()
class AOS_API AASGrenade : public AActor
{
	GENERATED_BODY()
	
	// Sets default values for this actor's properties
	AASGrenade();

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

private:
	UPROPERTY(EditAnywhere, Category = "Timer")
	int32 countDownTime;

	UPROPERTY(EditAnywhere, Category = "FX")
	class UParticleSystem* ExplosionParticles;

	UPROPERTY(EditAnywhere, Category = "FX")
	class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, Category = "Grenade")
	float DamageRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = "Grenade")
	float MinDamage = 50;

	UPROPERTY(EditAnywhere, Category = "Grenade")
	float MaxDamage = 200.f;

	FTimerHandle TimerHandle;

public:
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
