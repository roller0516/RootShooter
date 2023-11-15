// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASGameState.h"
#include "ASGameFrameWorkComponent.h"

AASGameState::AASGameState()
{
	FrameWorkComponent = CreateDefaultSubobject<UASGameFrameWorkComponent>(TEXT("FrameWorkComponent"));
}
