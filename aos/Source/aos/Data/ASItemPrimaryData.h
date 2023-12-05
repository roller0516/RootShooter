// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "../EnumTypes.h"
#include "ASItemPrimaryData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemBaseData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemBaseData(){};

	UPROPERTY(EditDefaultsOnly,Category = "Item")
	int32 ItemID;
	
	UPROPERTY(EditDefaultsOnly,Category = "Item")
	int32 Count;

	UPROPERTY(EditDefaultsOnly,Category = "Item")
	TObjectPtr<USkeletalMesh> mesh;

	UPROPERTY(EditDefaultsOnly,Category = "Item")
	TObjectPtr<UTexture2D> Texture;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FItemBaseData
{
	GENERATED_BODY()
public:
	FWeaponData(){};

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	WeaponType weaponType;

	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	int32 maxAmmoCount;

	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	int defaultMagazineCapacity;

	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	float MinDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float MaxDamage;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName ClipBoneName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<class UTexture2D> ammoTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<class UNiagaraSystem> MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<class UNiagaraSystem> ShellEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<class UNiagaraSystem> TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<class UStaticMesh> ShellMesh;

	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	TSubclassOf<class UAnimInstance> animInstance; 

	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	TObjectPtr<class UTexture2D> CrosshairsMiddle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<class UTexture2D> CrosshairsLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<class UTexture2D> CrosshairsRight;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<class UTexture2D> CrosshairsTop;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<class UTexture2D> CrosshairsBottom;
};

UCLASS()
class AOS_API UASItemPrimaryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "ItemTable")
	UDataTable* ItemTable;

	UPROPERTY(EditAnywhere, Category = "WeaponTable")
	UDataTable* WeaponTable;

	FItemBaseData* GetItemData(int32 itemID);
	FWeaponData* GetWeaponData(int32 itemID);
	
	virtual void PostLoad() override;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(FPrimaryAssetType("ASItemData"), FName("ItemData"));
	}
protected:
	void CashedItemDataTable();
private:
	TArray<FItemBaseData*> ItemDatas;
	TArray<FWeaponData*> WeaponDatas;
};
