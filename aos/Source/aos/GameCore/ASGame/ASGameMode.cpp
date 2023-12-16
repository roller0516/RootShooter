// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASGameMode.h"
#include "GameFramework/GameModeBase.h"
#include "Data/ASUserFacingExperienceDefinition.h"
#include "Kismet/GameplayStatics.h"
#include "GameCore/ASGame/ASExperienceStateComponent.h"
#include "GameCore/ASGame/ASWorldSettings.h"
#include "Data/ASExperienceDefinition.h"
#include "Data/ASPawnData.h"
#include "GameCore/ASGame/ASAssetManager.h"
#include "GameCore/ASGame/ASGameInstance.h"



void AASGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName,Options,ErrorMessage);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);

	FSlateApplication::Get().SetAllUserFocusToGameViewport();
}

void AASGameMode::InitGameState()
{
	Super::InitGameState();

	UASExperienceStateComponent* ExperienceComponent = GameState->FindComponentByClass<UASExperienceStateComponent>();
	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &AASGameMode::OnExperienceLoaded));
}

UClass* AASGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UASPawnData* PawnData = GetPawnData(InController))
	{	
		UASGameInstance* gi = Cast<UASGameInstance>(GetGameInstance());

		if (PawnData->pawnClass.Contains((gi->GetCharID())))
		{
			return PawnData->pawnClass[gi->GetCharID()];
		}
		return Super::GetDefaultPawnClassForController_Implementation(InController);
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

const UASPawnData* AASGameMode::GetPawnData(const AController* controller) const
{
	UASExperienceStateComponent* ExperienceComponent = GameState->FindComponentByClass<UASExperienceStateComponent>();

	if(ExperienceComponent->IsExperienceLoaded())
	{
		UASExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperience();
		if (Experience->pawnData != nullptr)
		{
			return Experience->pawnData;
		}
		//return UASAssetManager::Get().GetDefaultPawnData();
	}
	return nullptr;
}

void AASGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
	//Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	//Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void AASGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;

	UWorld* World = GetWorld();
	//월드 세팅에 Experience가 설정 되어있는 기준이 먼저
	if (!ExperienceId.IsValid())
	{
		AASWorldSettings* Settings = Cast<AASWorldSettings>(GetWorldSettings());
		ExperienceId = Settings->GetDefaultGameplayExperience();
		ExperienceIdSource = TEXT("WorldSettings");
	}

	if(!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("ASExperienceDefinition"),FName("DefaultExperience"));
		ExperienceIdSource = TEXT("Deafault");
	}

	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}

void AASGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
	UASExperienceStateComponent* ExperienceComponent = GameState->FindComponentByClass<UASExperienceStateComponent>();
	check(ExperienceComponent);
	ExperienceComponent->ServerSetCurrentExperience(ExperienceId,ExperienceIdSource);
}

void AASGameMode::OnExperienceLoaded(const UASExperienceDefinition* experience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
				//HandleStartingNewPlayer_Implementation(PC);
			}
		}
	}
}

bool AASGameMode::IsExperienceLoaded() const
{
	check(GameState);
	UASExperienceStateComponent* ExperienceComponent = GameState->FindComponentByClass<UASExperienceStateComponent>();
	check(ExperienceComponent);

	return ExperienceComponent->IsExperienceLoaded();
}

bool AASGameMode::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	return true;
}
