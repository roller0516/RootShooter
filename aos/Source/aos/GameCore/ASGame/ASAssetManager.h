// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ASAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AOS_API UASAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UASAssetManager& Get();
	
	FName GetName() const ;
protected:
	virtual void StartInitialLoading() override;
	
};
