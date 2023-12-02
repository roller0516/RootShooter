// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "ASItemData.generated.h"

/**
 * 
 */
 struct FItemBaseData;

UCLASS()
class AOS_API UASItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	FItemBaseData* UASItemData::GetItemData(int32 itemID);

	UPROPERTY(EditAnywhere,Category = "WeaponTable")
	UDataTable* weaponTable;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(FPrimaryAssetType("ASItemData"), "ItemData");
	}
};
