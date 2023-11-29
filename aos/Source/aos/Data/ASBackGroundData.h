// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ASBackGroundData.generated.h"

/**
 * 
 */


UCLASS(config = Input)
class AOS_API UASBackGroundData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,meta = (AllowedTypes = "Map"))
	TMap<int,TSoftObjectPtr<UWorld>> BackgroundLevel;

	FORCEINLINE TSoftObjectPtr<UWorld> GetMap(int ID) const
	{
		if(BackgroundLevel.Contains(ID))
			return BackgroundLevel[ID];
		
		return nullptr;
	}

	FORCEINLINE TSoftObjectPtr<UWorld> GetMap(TSoftObjectPtr<UWorld> Map) const
	{
		for (auto& var : BackgroundLevel)
		{
			if(var.Value == Map)
			{	
				return var.Value;
			}
		}
		return nullptr;
	}

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("BackGroundData", "MapData");
	}
};
