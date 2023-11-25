// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ASPawnData.generated.h"

/**
 * 
 */
UCLASS()
class AOS_API UASPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UASPawnData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APawn> PawnClass;
};
