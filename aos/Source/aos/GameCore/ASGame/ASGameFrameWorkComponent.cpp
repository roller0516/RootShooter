// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASGameFrameWorkComponent.h"
#include "Engine/AssetManager.h"

// Sets default values for this component's properties
UASGameFrameWorkComponent::UASGameFrameWorkComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UASGameFrameWorkComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

bool UASGameFrameWorkComponent::ShouldShowLoadingScreen(FString& OutReason) const
{
	return false;
}

void UASGameFrameWorkComponent::StartLoad()
{
	check(currentExperience != nullptr);
	check(currentFeartureState == EFeatureLoadState::Unload);

	currentFeartureState = EFeatureLoadState::Loading;

	UAssetManager& AssetManager = UAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;
	TSet<FSoftObjectPath> RawAssetList;

	//BundleAssetList.Add(currentExperience->GetPrimaryAssetId());
	//
	//for( TObjectPtr<UASUserFacingExperienceDefinition>& actionSet : currentExperience->ActionSets)
	//{
	//	
	//}

}

void UASGameFrameWorkComponent::OnLoadComplete()
{
	
}

void UASGameFrameWorkComponent::GameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
}

void UASGameFrameWorkComponent::FullLoadComplete()
{
}