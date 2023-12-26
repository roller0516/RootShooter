// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ASItemBase.h"
#include "ASGrenade.generated.h"

UCLASS()
class AOS_API AASGrenade : public AASItemBase
{
	GENERATED_BODY()
	
	// Sets default values for this actor's properties
public:
	AASGrenade();
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
protected:
	void Explosion();
public:
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float Damage;

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

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* OnHitCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector explosionEffectScale;

	TArray<AActor*> ignores;
};
