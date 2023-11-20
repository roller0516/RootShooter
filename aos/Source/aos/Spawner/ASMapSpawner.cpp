// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/ASMapSpawner.h"
#include "GameCore/ASGame/ASAssetManager.h"
#include "UObject/PrimaryAssetId.h"
#include "GameCore/CommonLoadingScreen/ASLoadingProcessTask.h"
#include "Data/ASBackGroundData.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"
#include "UObject/UObjectGlobals.h"
// Sets default values
AASMapSpawner::AASMapSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AASMapSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	ShouldShowLoadingScreen();

	UASAssetManager& Assetmanager = UASAssetManager::Get();

	if (Assetmanager.IsValid())
	{
		TArray<FName> Bundles;
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AASMapSpawner::PrimaryAssetLoadComplete);
		Assetmanager.LoadPrimaryAsset(MapLoadAsset, Bundles, Delegate);
	}
}

void AASMapSpawner::ShouldShowLoadingScreen()
{
	LoadingTask = UASLoadingProcessTask::CreateProcessTask(this,FString::Printf(TEXT("Loading BackGround")));
}

void AASMapSpawner::HideLoadingScreen()
{
	FTimerHandle handle;

	GetWorld()->GetTimerManager().SetTimer(handle,[this]()
	{
		LoadingTask->Unregister();
	},0.5f,false);
}

void AASMapSpawner::PrimaryAssetLoadComplete()
{
	AyncLevelLoad();
}

void AASMapSpawner::AyncLevelLoad()
{
	UASAssetManager& Assetmanager = UASAssetManager::Get();

	FSoftObjectPath AssetPath = Assetmanager.GetPrimaryAssetPath(MapLoadAsset);

	TSubclassOf<UASBackGroundData> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	const UASBackGroundData* backGroundData = GetDefault<UASBackGroundData>(AssetClass);

	FString LoadPackagePath = FPaths::GetBaseFilename(backGroundData->BackgroundLevel.ToString(),false);

	LoadPackageAsync(LoadPackagePath, FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
		{
			if (Result == EAsyncLoadingResult::Succeeded)
			{
				AyncLevelLoadFinished(backGroundData->BackgroundLevel->GetName());
			}
		}), 0, PKG_ContainsMap);
}

void AASMapSpawner::AyncLevelLoadFinished(const FString LevelName)
{
	UGameplayStatics::OpenLevel(this,FName(*LevelName));
}

