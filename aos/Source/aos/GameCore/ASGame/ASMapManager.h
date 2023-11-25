// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ASMapManager.generated.h"

/**
 * 맵 로드 하는곳.
 */

class UASLoadingProcessTask;
struct FPrimaryAssetId;

UCLASS()
class AOS_API UASMapManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:	
	UASMapManager();
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
private:
	void ShouldShowLoadingScreen();
	void HideLoadingScreen();
	void PrimaryAssetLoadComplete();

	UFUNCTION(BlueprintCallable)
	void AyncLevelLoad(int ID);

	void AyncLevelLoadFinished(const FString LevelName);
private:
	UPROPERTY()
	FPrimaryAssetId MapLoadAsset;

	UPROPERTY()
	UASLoadingProcessTask* LoadingTask;

	int mapID;


};
