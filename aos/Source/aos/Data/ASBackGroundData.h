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
		return BackgroundLevel[ID];
	}

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("BackGroundData", "MapData");
	}
};
