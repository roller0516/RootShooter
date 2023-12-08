// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ASBaseCharacter.h"
#include "ASPlayerCharacter.generated.h"

UENUM()
enum class MontageType :uint8
{
	Reloading,
	Attack,
};

UENUM()
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_FireTimerInProgress UMETA (DisplayName = "Fire"),
	ECS_UsedSkill UMETA(DisplayName = "UsedSkill"),
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

class UASInventoryComponent;

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
	virtual void PossessedBy(AController* NewController) override;

	void CharacterMove(const FInputActionValue& Value);
	void CharacterLook(const FInputActionValue& Value);

	void MouseLeftClick();
	//---------------------------------------------------------Attack
	void FireWeapon();
	void StartCrossHairBulletFire();
	void FinishCrossHairBulletFire();
	//---------------------------------------------------------Skill
	void CreateBarrier();
	void UseGrenadeSkill();
	void UseSkill();
	void BuildTypeSkillTrace(); // 설치형 스킬 Trace
	
	//---------------------------------------------------------Aiming
	void AimingButtonPressed();
	void AimingButtonReleased();
	void CameraInterpZoom(float deltaTime);
	void SetLookRates();
	void Aim();
	void StopAiming();
	//---------------------------------------------------------CrossHair
	void CalcCrossHairSpread(float deltaTime);
	void IncreaseSpread(float increaseAmount);
	void DecreaseSpread(float decreaseAmount);
	//---------------------------------------------------------Beam
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult);
	bool TraceUnderCrosshairs(FHitResult&OutHitResult,FVector& OutHitLocation);
	//---------------------------------------------------------Movement
	void CalcAimingSpeed();
	//---------------------------------------------------------EquipWeapon
	void EquipWeapon(class AASWeapon* WeaponToEquip);
	void DropWeapon(); // Detach
	AASWeapon* SpawnDefaultWeapon();
	//---------------------------------------------------------Reloading
	void Reloading();
	UFUNCTION(BlueprintCallable)
	void FinishReloading();
	//---------------------------------------------------------Ammo
	UFUNCTION(BlueprintCallable)
	void SwapWeapon(class AASWeapon* swapWeapon);

	bool WeaponHasAmmo();

	UFUNCTION(BlueprintCallable)
	void GrapClip();
	
	UFUNCTION(BlueprintCallable)
	void ReplaceClip();

	void PickUp();

	void GetPickupItem(class AASItemBase* item);
private :
	//---------------------------------------------------------Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAReloading;
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IARooting;
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
	//UPROPERTY(EditAnywhere, Category = Montage)
	//class UAnimMontage* HipFireMontage;

	UPROPERTY(EditAnywhere, Category = Montage)
	TMap<MontageType,UAnimMontage*> Montages;
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

	bool bAimingButtonPressed;

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

	float aimingMovementSpeed;

	//-------------------------------------------------------AttackSpeed
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackDelayTime;

	//-------------------------------------------------------BuildTypeSkill
	UPROPERTY(EditAnywhere, Category = Skill)
	float TraceDistance;

	FVector GroundPlacementPoint;
	//-------------------------------------------------------Weapon
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class AASWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere,Category = Weapon)
	TSubclassOf<class AASWeapon> DefaultWeapon;

	//-------------------------------------------------------Clip
	FTransform ClipTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,meta=(AllowPrivateAccess = "true"))
	USceneComponent* HandSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UASInventoryComponent> InventoryComponent;

	//-------------------------------------------------------Item
	UPROPERTY()
	class AASItemBase* rootingItem;



protected:
	//-------------------------------------------------------Skill
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> barrierActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> grenadeActor;
public:
	FORCEINLINE float GetCrossHairSpreadMultiplier() const {return CrossHairSpreadMultiplier;}
	FORCEINLINE bool GetAiming() const {return bIsAiming;}
	FORCEINLINE ECombatState GetCombatState() { return CombatState; }
	FORCEINLINE class AASWeapon* GetEquipWeapon(){ return EquippedWeapon;}
};
