// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASGameInstance.h"
#include "../../../Experimental/ModularGameplay/Source/ModularGameplay/Public/Components/GameFrameworkComponentManager.h"


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
}

void UASGameInstance::Shutdown()
{
	Super::Shutdown();
}
