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
	//AASWeapon(int32 itemid);
	void ShowShotParticles(FHitResult pHitResult);
	FTransform GetBarrelSocketTransForm() const;

	void DecrementAmmo();
	void CreateWeapon(int32 itemID);

	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move; }
	FORCEINLINE int GetCurrentAmmoCount(){return curAmmonCount;}
protected:
	void SetTexture() override;
	void SetMesh() override;
	void SetCount() override;
	void BeginPlay() override;
	void PostInitializeComponents() override;
	//void SetAmmoCount(int32 count);
private:
	void ResetCurAmmoCount(int32 count);
	void SetMaxAmmoCount();

public:
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	WeaponType weaponType = WeaponType::None;

private:
	UPROPERTY(EditAnywhere, Category = "Wepon Properties", meta=(AllowPrivateAccess = "true"))
	FName ClipBoneName;

	FName BarrelSocketName;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Wepon Properties",meta = (AllowPrivateAccess = "true"))
	bool bMovingClip;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UNiagaraSystem> MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UNiagaraSystem> ShellEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UNiagaraSystem> TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UStaticMesh> ShellMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int maxAmmoCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float MinDamage = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float MaxDamage = 12;

public:
	void ShowShotParticles(FHitResult pHitResult);
	FTransform GetBarrelSocketTransForm() const;

	FORCEINLINE FName GetClipBoneName() const {return ClipBoneName;}
	FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move; }
	FORCEINLINE float GetDamage();
	FORCEINLINE float GetHeadDamage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int curAmmonCount;

	FWeaponData* weaponData;
};
