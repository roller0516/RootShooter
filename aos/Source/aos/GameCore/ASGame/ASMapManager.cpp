// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASMapManager.h"
#include "ASAssetManager.h"
#include "Engine/StreamableManager.h"
#include "Data/ASBackGroundData.h"
#include "Kismet/GameplayStatics.h"
#include "../CommonLoadingScreen/ASLoadingProcessTask.h"

UASMapManager::UASMapManager()
{

}

void UASMapManager::ShouldShowLoadingScreen()
{
	LoadingTask = UASLoadingProcessTask::CreateProcessTask(this, FString::Printf(TEXT("Loading BackGround")));
}

void UASMapManager::HideLoadingScreen()
{
	FTimerHandle handle;

	GetWorld()->GetTimerManager().SetTimer(handle, [this]()
		{
			LoadingTask->Unregister();
		}, 0.5f, false);
}

void UASMapManager::PrimaryAssetLoadComplete()
{
	UASAssetManager& Assetmanager = UASAssetManager::Get();

	FSoftObjectPath AssetPath = Assetmanager.GetPrimaryAssetPath(MapLoadAsset);

	TSubclassOf<UASBackGroundData> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	const UASBackGroundData* backGroundData = GetDefault<UASBackGroundData>(AssetClass);

	FString LoadPackagePath = FPaths::GetBaseFilename(backGroundData->GetMap(mapID).ToString(), false);

	LoadPackageAsync(LoadPackagePath, FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
		{
			if (Result == EAsyncLoadingResult::Succeeded)
			{
				AsyncLevelLoadFinished(backGroundData->GetMap(mapID)->GetName());
			}
		}), 0, PKG_ContainsMap);
}

void UASMapManager::AsyncLevelLoad(int ID)
{
	mapID = ID;

	ShouldShowLoadingScreen();

	UASAssetManager& AssetManager = UASAssetManager::Get();
	if (AssetManager.IsValid())
	{
		TArray<FName> Bundles;
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UASMapManager::PrimaryAssetLoadComplete);
		AssetManager.LoadPrimaryAsset(MapLoadAsset, Bundles, Delegate);
	}
}

void UASMapManager::AsyncLevelLoadFinished(const FString LevelName)
{
	UGameplayStatics::OpenLevel(this, FName(*LevelName));

	HideLoadingScreen();
}

void UASMapManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UASAssetManager& assetManager = UASAssetManager::Get();

	MapLoadAsset = FPrimaryAssetId(FPrimaryAssetType("BackGroundData"), FName("MapData"));
}
