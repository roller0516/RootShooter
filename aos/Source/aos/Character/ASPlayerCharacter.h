// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ASBaseCharacter.h"
#include "ASPlayerCharacter.generated.h"

UCLASS()
class AOS_API AASPlayerCharacter : public AASBaseCharacter
{
	GENERATED_BODY()

public:
	AASPlayerCharacter();
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;
	
	void CharacterMove(const FInputActionValue& Value);
	void CharacterLook(const FInputActionValue& Value);

	void MouseLeftClick();
	//---------------------------------------------------------Attack
	void FireWeapon();
	void StartCrossHairBulletFire();
	void FinishCrossHairBulletFire();
	//---------------------------------------------------------Skill
	void CreateBarrier();
	void UseSkill();
	void BuildTypeSkillTrace(); // 설치형 스킬 Trace
	
	//---------------------------------------------------------Aiming
	void AimingButtonPressed();
	void AimingButtonReleased();
	void CameraInterpZoom(float deltaTime);
	void SetLookRates();
	//---------------------------------------------------------CrossHair
	void CalcCrossHairSpread(float deltaTime);
	void IncreaseSpread(float increaseAmount);
	void DecreaseSpread(float decreaseAmount);
	//---------------------------------------------------------Beam
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);
	//---------------------------------------------------------Movement
	void CalcAimingSpeed();
	//---------------------------------------------------------EquipWeapon
	void EquipWeapon();
private :
	//---------------------------------------------------------Camera
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;

	//---------------------------------------------------------Input
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* defaultMappingContext;
	 
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAMove;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IALook;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAJump;
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAAttack;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IASkill1;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IASkill2;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAAiming;
	
	//--------------------------------------------------------particle
	//TODO::data
	UPROPERTY(EditAnywhere, Category = Particle)
	class UParticleSystem* MuzzleFlash;
	
	UPROPERTY(EditAnywhere, Category = Particle)
	class UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, Category = Particle)
	class UParticleSystem* ShotLineParticle;
	//--------------------------------------------------------Montage

	//TODO:: data 
	UPROPERTY(EditAnywhere, Category = Motage)
	class UAnimMontage* HipFireMontage;
	
	//-------------------------------------------------------Aiming
	UPROPERTY(VisibleAnywhere,Category = Aiming)
	bool bIsAiming;

	UPROPERTY(EditAnywhere, Category = Aiming)
	float CameraDefaultFOV;
	
	UPROPERTY(EditAnywhere, Category = Aiming)
	float CameraZoomedFOV;

	UPROPERTY(EditAnywhere, Category = Aiming)
	float ZoomSpeed;

	float CurrentCameraFOV;
	
	UPROPERTY(EditAnywhere,Category = Aiming)
	float BaseTurnRate;

	UPROPERTY(EditAnywhere,Category = Aiming)
	float BaseLookUpRate;
	//TODO::마우스 감도를 조절 할 수있는 UI만들면 그쪽으로 빼자.
	UPROPERTY(EditAnywhere,Category=Aiming)
	float HipTurnRate;
	
	UPROPERTY(EditAnywhere,Category=Aiming)
	float HipLookUpRate;
	
	UPROPERTY(EditAnywhere,Category=Aiming)
	float AimingTurnRate;
	
	UPROPERTY(EditAnywhere,Category=Aiming)
	float AimingLookUpRate;

	//-------------------------------------------------------CrossHair
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CrossHairs, meta = (AllowPrivateAccess = "true"))
	float CrossHairSpreadMultiplier;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CrossHairs, meta = (AllowPrivateAccess = "true"))
	float CrossHairVelocityFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CrossHairs, meta = (AllowPrivateAccess = "true"))
	float CrossHairInAirFactor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CrossHairs, meta = (AllowPrivateAccess = "true"))
	float CrossHairAimFactor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CrossHairs, meta = (AllowPrivateAccess = "true"))
	float CrossHairShootingFactor;

	UPROPERTY(EditAnywhere,Category = CrossHairs)
	float SpreadMin;

	UPROPERTY(EditAnywhere,Category = CrossHairs)
	float SpreadHipMax;
	
	UPROPERTY(EditAnywhere,Category = CrossHairs)
	float SpreadAimingMax;
	
	UPROPERTY(EditAnywhere,Category = CrossHairs)
	float SpreadCurrent;

	UPROPERTY(EditAnywhere, Category = CrossHairs)
	float SpreadDecreaseSpeed;

	UPROPERTY(EditAnywhere, Category = CrossHairs)
	float SpreadIncreaseSpeed;
	//-------------------------------------------------------ShootingTime
	UPROPERTY(EditAnywhere,Category = ShootTimer)
	float ShootTimeDuration;

	bool bFiringBullet;

	FTimerHandle CrossHairShootTimer;
	
	//-------------------------------------------------------Move
	float defaultMovementSpeed;

	//-------------------------------------------------------AttackSpeed
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackDelayTime;

	//-------------------------------------------------------BuildTypeSkill
	UPROPERTY(EditAnywhere, Category = Skill)
	float TraceDistance;

	bool bUseSkill;

	FVector GroundPlacementPoint;
protected:
	//-------------------------------------------------------Skill
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> barrierActor;
public:
	FORCEINLINE float GetCrossHairSpreadMultiplier() const {return CrossHairSpreadMultiplier;}
	FORCEINLINE bool GetAiming() const {return bIsAiming;}
};
