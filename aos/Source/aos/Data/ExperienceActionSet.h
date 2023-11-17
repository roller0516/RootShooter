// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ExperienceActionSet.generated.h"

/**
 * 
 */
class UGameFeatureAction;
UCLASS()
class AOS_API UExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UExperienceActionSet();

public:
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
