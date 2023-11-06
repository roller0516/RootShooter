// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASBulletHitIInterface.h"
#include "ASEnemy.generated.h"

UCLASS()
class AOS_API AASEnemy : public ACharacter, public IASBulletHitIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AASEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// �Ѿ˿� �¾��� �� ��ƼŬ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	class UParticleSystem* ImpactParticles;

	// �Ѿ˿� Ÿ�ݴ����� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	class USoundCue* ImpactSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult) override;

};
