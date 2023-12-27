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
	Grenade,
	Reloading,
	Attack,
	Equip,
};

UENUM()
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_FireTimerInProgress UMETA (DisplayName = "Fire"),
	ECS_UsedSkill UMETA(DisplayName = "UsedSkill"),
	ECS_Equip UMETA(DisplayName = "Equip"),
	ECS_Grenade UMETA(DisplayName = "Grenade"),
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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void Heal(int Amount);
	void GetAmmo(int Amount);

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void CharacterMove(const FInputActionValue& Value);
	void CharacterLook(const FInputActionValue& Value);
	void ClearSpline();
	void MouseLeftClick();
	void ChangeCombatState(ECombatState changeState);
	//---------------------------------------------------------Attack
	void FireWeapon();
	void StartCrossHairBulletFire();
	void FinishCrossHairBulletFire();
	//---------------------------------------------------------Skill
	void CreateBarrier();
	void SetGrenadeSkill();
	void UseGrenadeSkill();
	void DrawGrenadePath();

	UFUNCTION(BlueprintCallable)
	void SpawnGrenade();
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

	UFUNCTION()
	void ChangeMouseRate(float turnRate, float lookUpRate);
	UFUNCTION()
	void ChangeAmingMouseRate(float turnRate, float lookUpRate);
	//---------------------------------------------------------Beam
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult);
	bool TraceUnderCrosshairs(FHitResult&OutHitResult,FVector& OutHitLocation);
	//---------------------------------------------------------Movement
	void CalcAimingSpeed();
	//---------------------------------------------------------EquipWeapon
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(class AASWeapon* WeaponToEquip);
	void DropWeapon(); // Detach
	AASWeapon* SpawnWeapon(AASWeapon* copyItem);

	void ChangeWeapon1();
	void ChangeWeapon2();
	void ChangeWeapon3();

	void ExchangeInventoryItems(int32 currentItemidx,int32 newItemidx);

	UFUNCTION(BlueprintCallable)
	void FinishEquip();
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

	UFUNCTION(BlueprintCallable)
	void FinishGrenade();

	void Cloacking();

	UFUNCTION(BlueprintCallable)
	void EnableCustomDepth(AActor* TouchedActor);
	UFUNCTION(BlueprintCallable)
	void DisableCustomDepth(AActor* TouchedActor);

	void FinialArcSpawn(FVector location);
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
	class UInputAction* IASkill3;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAAiming;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAReloading;
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IARooting;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAWeaponSlot1;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAWeaponSlot2;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IAWeaponSlot3;
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

	FVector TargetPoint;

	FRotator TargetRotator;

	FVector ImpactPoint;

	FRotator ImpactRotator;

	UPROPERTY(VisibleAnywhere)
	class USplineComponent* splinComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* splineMesh;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* splineMeshMeterial;

	UPROPERTY(VisibleAnywhere)
	TArray<class USplineMeshComponent* > splineMeshs;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> FinalArcSubclass;

	AActor* FinalArc;
	//-------------------------------------------------------ShootingTime
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

	UPROPERTY(BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	int currentWeaponSlot;

	//-------------------------------------------------------BloodParticle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BloodParticles;

	//-------------------------------------------------------Cloacking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UMaterial* CloackingMaterial;

	UPROPERTY()
	TArray<UMaterialInterface*> SaveMaterials;

	bool isCloacking;

	//-------------------------------------------------------HP
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = combat, meta = (AllowPrivateAccess = "true"));
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	float MaxHealth;

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
	FORCEINLINE UParticleSystem* GetBloodParticles() const { return BloodParticles; }
	FORCEINLINE bool GetCloacking() const { return isCloacking; }
};
