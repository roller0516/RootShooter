// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASAssetManager.h"
#include "GameCore/ASGame/FASGameplayTags.h"

UASAssetManager& UASAssetManager::Get()
{
	check(GEngine);

	return*Cast<UASAssetManager>(GEngine->AssetManager);
}

void UASAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FASGameplayTags::InitializeNativeGameplayTags();

	//UAbilitySystemGlobals::Get().InitGlobalData();
}
