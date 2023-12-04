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

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

	void Die();

	void PlayHitMontage(FName Section, float PlayRate = 1.0f);

	void ResetHitReactTimer();

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber, FVector Location);

	UFUNCTION()
	void DestroyHitNumber(UUserWidget* HitNumber);

	void UpdateHitNumbers();

private:
	// �Ѿ˿� �¾��� �� ��ƼŬ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	class UParticleSystem* ImpactParticles;

	// �Ѿ˿� Ÿ�ݴ����� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	class USoundCue* ImpactSound;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = combat, meta = (AllowPrivateAccess = "true"));
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	FString HeadBone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	float HealthBarDisplayTime;

	FTimerHandle HealthBarTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	UAnimMontage* HitMontage;

	FTimerHandle HitReactTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	float HitReactTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	float HitReactTimeMax;

	bool bCanHitReact;

	UPROPERTY(VisibleAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"));
	TMap<UUserWidget*, FVector> HitNumbers;

	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"));
	float HitNumberDestroyTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE FString GetHeadBone() const { return HeadBone; }

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 Damage, FVector HitLocation, bool bHeadShot);
};
