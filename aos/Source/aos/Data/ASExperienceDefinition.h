// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ASExperienceDefinition.generated.h"

/**
 * 
 */

class UGameFeatureAction; 
class UExperienceActionSet;
//class FString;
UCLASS(BlueprintType,Const)
class AOS_API UASExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UASExperienceDefinition();

	UPROPERTY(EditDefaultsOnly)
	TArray<FString> GameFeaturesToEnble;

	UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<TObjectPtr<UExperienceActionSet>> ActionSets;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ASExperienceDefinition",GetFName());
	}
};
