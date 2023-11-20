// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Command/ILoadingProcessInterFace.h"
#include "ASMapSpawner.generated.h"

class UASLoadingProcessTask;
struct FPrimaryAssetId;

UCLASS()
class AOS_API AASMapSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AASMapSpawner();
protected:
	virtual void BeginPlay() override;
private:
	void ShouldShowLoadingScreen();
	void HideLoadingScreen();
	void PrimaryAssetLoadComplete();

	void AyncLevelLoad();
	void AyncLevelLoadFinished(const FString LevelName);
private:
	UPROPERTY(EditAnywhere, Category = "MapData", meta = (AllowedTypes = "BackGroundData"))
	FPrimaryAssetId MapLoadAsset;

	UASLoadingProcessTask* LoadingTask;
};
