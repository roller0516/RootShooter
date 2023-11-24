// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASPlayerController.h"
#include "GameCore/CommonGame/ASCommonLocalPlayer.h"
#include "UI/UICore/ASPrimaryGameLayout.h"
#include "GameCore/ASGame/FASGamePlayTags.h"
#include "Input/CommonUIActionRouterBase.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/CommonActivatableWidget.h"

AASPlayerController::AASPlayerController(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AASPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	//ReceivedPlayer();
}

void AASPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AASPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (UASCommonLocalPlayer* LocalPlayer = Cast<UASCommonLocalPlayer>(Player))
	{
		LocalPlayer->onPlayerControllerSet.Broadcast(LocalPlayer, this);

		if (PlayerState)
		{
			LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);
		}
	}
}

void AASPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AASPlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);

	//if(const uaslo)
}

void AASPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AASPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
