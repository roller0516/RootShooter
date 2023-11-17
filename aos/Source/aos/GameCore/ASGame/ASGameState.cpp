// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASGameState.h"
#include "ASExperienceStateComponent.h"

AASGameState::AASGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ExperienceState = CreateDefaultSubobject<UASExperienceStateComponent>(TEXT("ExperienceStateComponent"));
}

void AASGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AASGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AASGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
