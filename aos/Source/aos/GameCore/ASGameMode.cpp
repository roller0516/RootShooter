// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGameMode.h"
#include "GameFramework/GameModeBase.h"
#include "Data/ASUserFacingExperienceDefinition.h"
#include "Kismet/GameplayStatics.h"
#include "ASGame/ASExperienceStateComponent.h"
#include "ASGame/ASWorldSettings.h"


void AASGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName,Options,ErrorMessage);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);

	FSlateApplication::Get().SetAllUserFocusToGameViewport();
}

void AASGameMode::InitGameState()
{
	Super::InitGameState();
}

void AASGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;

	UWorld* World = GetWorld();

	//if(!ExperienceId.IsValid())
	//{
	//	ExperienceId = FPrimaryAssetId(FPrimaryAssetType("ASExperienceDefinition"),FName("BP_DefaultExperience"));
	//	ExperienceIdSource = TEXT("Deafault");
	//}

	if(!ExperienceId.IsValid())
	{
		AASWorldSettings* Settings = Cast<AASWorldSettings>(GetWorldSettings());
		ExperienceId = Settings->GetDefaultGameplayExperience();
		ExperienceIdSource = TEXT("WorldSettings");
	}

	
	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}

void AASGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
	UASExperienceStateComponent* ExperienceComponent = GameState->FindComponentByClass<UASExperienceStateComponent>();
	check(ExperienceComponent);
	ExperienceComponent->ServerSetCurrentExperience(ExperienceId,ExperienceIdSource);
}
