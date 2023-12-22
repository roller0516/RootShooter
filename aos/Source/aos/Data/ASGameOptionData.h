// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ASGameOptionData.generated.h"

/**
 * 
 */

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnaa,float,a,float,b);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMouseRateChange, float, mouseTurnRate, float, mouseLookUpRate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAimingMouseRateChange,float, AimingMouseTurnRate,float, AimingMouseLookUpRate);

UCLASS()
class AOS_API UASGameOptionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite,Category = Mouse, meta = (ClampMin = 0,ClampMax = 100))
	float MouseTurnRate;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Mouse, meta = (ClampMin = 0, ClampMax = 100))
	float MouseLookUpRate;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Mouse, meta = (ClampMin = 0, ClampMax = 100))
	float AimingMouseTurnRate;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Mouse, meta = (ClampMin = 0, ClampMax = 100))
	float AimingMouseLookUpRate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnMouseRateChange OnMouseRateChange;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAimingMouseRateChange OnAimingMouseRateChange;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("GameOptionData", "GameOptionData");
	}
};
