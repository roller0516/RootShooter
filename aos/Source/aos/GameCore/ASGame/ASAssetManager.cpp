// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASAssetManager.h"
#include "GameCore/ASGame/FASGameplayTags.h"
#include "Templates/UnrealTemplate.h"

UASAssetManager& UASAssetManager::Get()
{
	check(GEngine);

	if(UASAssetManager* singleTon = Cast<UASAssetManager>(GEngine->AssetManager))
	{
		return *singleTon;
	}

	return*NewObject<UASAssetManager>();
}

//UASPawnData* UASAssetManager::GetDefaultPawnData()
//{
//	return GetAsset(DefaultPawnData);
//}


FName UASAssetManager::GetName() const
{
	return FName();
}


void UASAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FASGameplayTags::InitializeNativeGameplayTags();

	//UAbilitySystemGlobals::Get().InitGlobalData();
}