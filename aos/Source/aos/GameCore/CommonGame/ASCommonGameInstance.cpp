// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/CommonGame/ASCommonGameInstance.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "../CommonUser/ASCommonSubSystem.h"

DEFINE_LOG_CATEGORY(LogCommonGame);

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
	return Super::AddLocalPlayer(NewPlayer, UserId);
}

bool UASCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if(PrimaryPlayer == ExistingPlayer)
	{
		PrimaryPlayer.Reset();
		UE_LOG(LogCommonGame,Log,TEXT("RemoveLocalPlayer : Unsetting Primary Player from %s"),*ExistingPlayer->GetName());
	}
	//GetSubsystem<ugameuimana>

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
