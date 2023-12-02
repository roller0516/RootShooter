// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
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
	TSoftObjectPtr<USkeletalMesh> mesh;

	UPROPERTY(EditDefaultsOnly,Category = "Item")
	TSoftObjectPtr<UTexture> Texture;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FItemBaseData
{
	GENERATED_BODY()
public:
	FWeaponData(){};

	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	int32 maxAmmoCount;

	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	float Damage;
};

UCLASS()
class AOS_API UASItemPrimaryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "WeaponTable")
	UDataTable* weaponTable;

	FItemBaseData* GetItemData(int32 itemID);


	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(FPrimaryAssetType("ASItemData"),FName("ItemData"));
	}

};
