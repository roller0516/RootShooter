// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASItemBase.h"
#include "EnumTypes.h"
#include "ASWeapon.generated.h"

/**
 * 
 */
struct FWeaponData;
UCLASS()
class AOS_API AASWeapon : public AASItemBase
{
	GENERATED_BODY()
public:
	AASWeapon();

	virtual void CopyItem(AASItemBase* weapon) override;
	virtual void CreateItem(int32 _itemID) override;
	virtual void RefreshItem() override;

	void IncreaseAmmo(int32 AddAmmo);
	void DecrementAmmo();

	void SetCloacking(UMaterial* CloackingMaterial);
	void SetUnCloacking();

	UFUNCTION(BlueprintCallable)
	void ResetAmmo();

	void ShowShotParticles(FHitResult pHitResult);

	void SetActive(bool active);
	FTransform GetRightHandSocket();
	FName GetReloadMotageSection();
	void PlayFireSound();

	FORCEINLINE float GetShootTimeDuration(){return weaponData.ShootTimeDuration; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move; }
	FORCEINLINE float GetDamage();
	FORCEINLINE float GetHeadDamage();
	FORCEINLINE int GetCurrentAmmoCount() { return curAmmonCount; }
	FORCEINLINE void SetIsEquip(bool _isEquip) {IsEquip = _isEquip;}
	FORCEINLINE void SetIsUsed(bool _isUsed) { IsUsed = _isUsed;}
	FORCEINLINE bool GetIsEquip(bool _isUsed) { return IsEquip;}
	FORCEINLINE bool GetIsUsed(bool _isUsed) { return IsUsed; }
	FORCEINLINE bool IsAmmoFull(){return curAmmonCount == maxAmmoCount; }
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void SetTexture() override;
	virtual void SetMesh() override;
	virtual void SetCount() override;
	virtual void SetDamage();
	virtual void SetSound() override;
	virtual void BeginPlay() override;
	virtual	void UpdateItem() override;
	
	virtual void PostInitializeComponents() override;

	//void SetAmmoCount(int32 count);
private:
	void SetAnimInstance();
	void SetMaxAmmoCount();
	void SetMagazine();
	void SetCrossHair();

public:
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	WeaponType weaponType = WeaponType::None;

	FTransform GetBarrelSocketTransForm() const;

private:
	UPROPERTY(VisibleAnywhere, Category = "Wepon Properties", meta=(AllowPrivateAccess = "true"))
	FName ClipBoneName;

	FName BarrelSocketName;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Wepon Properties",meta = (AllowPrivateAccess = "true"))
	bool bMovingClip;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<class UNiagaraSystem> MuzzleEffect;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<class UNiagaraSystem> ShellEffect;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<class UNiagaraSystem> TracerEffect;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<class UStaticMesh> ShellMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int curAmmonCount;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int maxAmmoCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int magazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTexture2D* AmmoItemTexture;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FWeaponData weaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTexture2D> CrosshairsMiddle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTexture2D> CrosshairsLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTexture2D> CrosshairsRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTexture2D> CrosshairsTop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTexture2D> CrosshairsBottom;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsEquip;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsUsed;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* FireSound;

	UPROPERTY()
	TArray<UMaterialInterface*> SaveMaterials;
};
