// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ASAssetManager.generated.h"

/**
 * 
 */
struct  FPrimaryAssetData;

UCLASS()
class AOS_API UASAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UASAssetManager& Get();
	FName GetName() const ;
protected:
	virtual void StartInitialLoading() override;
	virtual void FinishInitialLoading() override;
public:
	template<typename T>
	TObjectPtr<T> GetPrimaryData(FPrimaryAssetId primaryID)
	{
		FSoftObjectPath AssetPath = GetPrimaryAssetPath(primaryID);
		TSubclassOf<T> AssetClass = Cast<UClass>(AssetPath.TryLoad());

		check(AssetClass);
		const TObjectPtr<T> Assetobject = AssetClass.GetDefaultObject();
		check(Assetobject);

		return Assetobject;
	}
};
