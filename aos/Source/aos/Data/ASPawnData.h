// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../EnumTypes.h"
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
	TMap<CharID,TSubclassOf<class APawn>> pawnClass;
};
