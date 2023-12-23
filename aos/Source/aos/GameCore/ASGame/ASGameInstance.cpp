// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASGameInstance.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameCore/ASGame/ASAssetManager.h"
#include "Data/ASItemPrimaryData.h"
#include "Data/ASGameOptionData.h"


UASGameInstance::UASGameInstance(const FObjectInitializer& ObjectInitializer) 
:Super(ObjectInitializer)
{

}

void UASGameInstance::Init()
{
	Super::Init();

	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	if(ensure(ComponentManager))
	{
		//const FLyraGameplayTags& GameplayTags = FLyraGameplayTags::Get();
		//
		//ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		//ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		//ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		//ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}


	UASAssetManager& assetManager = UASAssetManager::Get();
	ItemData = assetManager.GetPrimaryData<UASItemPrimaryData>(FPrimaryAssetId(FPrimaryAssetType("ASItemData"), FName("ItemData")));
	GameOpitionData = assetManager.GetPrimaryData<UASGameOptionData>(FPrimaryAssetId(FPrimaryAssetType("GameOptionData"), FName("GameOptionData")));
}

void UASGameInstance::Shutdown()
{
	Super::Shutdown();
}
