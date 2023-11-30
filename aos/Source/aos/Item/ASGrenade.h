// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASGrenade.generated.h"

UCLASS()
class AOS_API AASGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASGrenade();

private:
	UPROPERTY(EditAnywhere, Category = "Timer")
	int32 countDownTime;

	UPROPERTY(EditAnywhere, Category = "FX")
	class UParticleSystem* ExplosionParticles;

	UPROPERTY(EditAnywhere, Category = "FX")
	class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, Category = "Grenade")
	float Radius = 500.f;

	UPROPERTY(VisibleAnywhere)
	class UStaticMesh* grenadeMesh;

	FTimerHandle TimerHandle;

	UStaticMeshComponent* staticMeshCompoenent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Explode();
};
