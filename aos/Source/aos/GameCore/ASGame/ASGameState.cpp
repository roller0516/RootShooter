// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASGameState.h"
#include "ASExperienceStateComponent.h"
#include "../../../Experimental/ModularGameplay/Source/ModularGameplay/Public/Components/GameFrameworkComponentManager.h"

AASGameState::AASGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ExperienceState = CreateDefaultSubobject<UASExperienceStateComponent>(TEXT("ExperienceStateComponent"));
}

void AASGameState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void AASGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AASGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AASGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
