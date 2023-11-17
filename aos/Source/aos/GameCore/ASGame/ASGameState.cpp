// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASGameState.h"
#include "ASExperienceStateComponent.h"

AASGameState::AASGameState()
{
	ExperienceState = CreateDefaultSubobject<UASExperienceStateComponent>(TEXT("ExperienceStateComponent"));
}
