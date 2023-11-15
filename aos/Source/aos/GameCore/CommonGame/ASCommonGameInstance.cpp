// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/CommonGame/ASCommonGameInstance.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "../CommonUser/ASCommonSubSystem.h"
#include "ASGameUIManagerSubSystem.h"
#include "ASCommonLocalPlayer.h"
#include "Engine/LocalPlayer.h"


//DEFINE_LOG_CATEGORY(LogCommonGame);

UASCommonGameInstance::UASCommonGameInstance(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

void UASCommonGameInstance::HandleSystemMessage(FGameplayTag MessageType, FText Title, FText Message)
{
	ULocalPlayer* player = GetFirstGamePlayer();
	if (player && MessageType.MatchesTag(FCommonUserTags::SystemMessage_Error))
	{

	}
}

int32 UASCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 returnValue = Super::AddLocalPlayer(NewPlayer,UserId);
	if(returnValue != INDEX_NONE) // -1
	{
		if(!PrimaryPlayer.IsValid())
		{
			PrimaryPlayer = NewPlayer;
		}
		UASGameUIManagerSubSystem* uisubsystem = GetSubsystem<UASGameUIManagerSubSystem>();
		GetSubsystem<UASGameUIManagerSubSystem>()->NotifyAddPlayer(Cast<UASCommonLocalPlayer>(NewPlayer));
	}
	return returnValue;
}

bool UASCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if(PrimaryPlayer == ExistingPlayer)
	{
		PrimaryPlayer.Reset();
		UE_LOG(LogTemp,Log,TEXT("RemoveLocalPlayer : Unsetting Primary Player from %s"),*ExistingPlayer->GetName());
	}

	GetSubsystem<UASGameUIManagerSubSystem>()->NotifyRemovePlayer(Cast<UASCommonLocalPlayer>(ExistingPlayer));

	return Super::RemoveLocalPlayer(ExistingPlayer);
}

void UASCommonGameInstance::Init()
{
	Super::Init();
	UASCommonSubSystem* userSubSystem = GetSubsystem<UASCommonSubSystem>();
	if(userSubSystem)
	{
		
	}
}

void UASCommonGameInstance::ReturnToMainMenu()
{
	Super::ReturnToMainMenu();
}
