// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASPlayerController.h"
#include "GameCore/CommonGame/ASCommonLocalPlayer.h"
#include "UI/UICore/ASPrimaryGameLayout.h"
#include "GameCore/ASGame/FASGamePlayTags.h"
#include "Input/CommonUIActionRouterBase.h"
#include "CommonActivatableWidget.h"
#include "EnhancedPlayerInput.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Data/ASSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameCore/ASGame/ASGameInstance.h"
#include "Data/ASGameOptionData.h"
#include "Sound/SoundMix.h"
#include "AudioDeviceManager.h"
#include "AudioDevice.h"

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
}

void AASPlayerController::BeginPlay()
{
	Super::BeginPlay();
	InitSoundMix();
}

void AASPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AASPlayerController::SaveOptionControllData()
{
	UASSaveGame* game = Cast<UASSaveGame>(UGameplayStatics::CreateSaveGameObject(UASSaveGame::StaticClass()));

	game->SaveSlotName = "GameOptionData_Controller";
	game->SaveIndex = 0;

	UASGameInstance* gameInstance = Cast<UASGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	game->AimingMouseTurnRate = gameInstance->GameOpitionData->AimingMouseTurnRate;
	game->MouseTurnRate = gameInstance->GameOpitionData->MouseTurnRate;

	UGameplayStatics::SaveGameToSlot(game, game->SaveSlotName, game->SaveIndex);
}

void AASPlayerController::SaveOptionAudioData(float volume, float UI, float VoiceChat, float SFX , float Music)
{
	UASSaveGame* game = Cast<UASSaveGame>(UGameplayStatics::CreateSaveGameObject(UASSaveGame::StaticClass()));

	game->SaveSlotName = "GameOptionData_Audio";
	game->SaveIndex = 0;

	if (soundMix)
	{
		for (int i = 0; i < soundMix->SoundClassEffects.Num(); i++)
		{
			FString soundName = soundMix->SoundClassEffects[i].SoundClassObject.GetName();

			if (soundName.Equals(FString("Music")))
			{
				game->soundOption.MusicSoundVolum = Music;
			}
			else if (soundName.Equals(FString("Overall")))
			{
				game->soundOption.VolumeSoundVolum = volume;
			}
			else if (soundName.Equals(FString("SFX")))
			{
				game->soundOption.SFXSoundVolum = SFX;
			}
			else if (soundName.Equals(FString("UI")))
			{
				game->soundOption.UISoundVolum = UI;
			}
			else if (soundName.Equals(FString("VoiceChat")))
			{
				game->soundOption.VoiceChatSoundVolum = VoiceChat;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Not Find Sound , Sound Name : %s"), *soundName);
			}
		}
	}

	UGameplayStatics::SaveGameToSlot(game, game->SaveSlotName, game->SaveIndex);
}

UASSaveGame* AASPlayerController::LoadData(FString SaveSlotName)
{
	UASSaveGame* game = Cast<UASSaveGame>(UGameplayStatics::CreateSaveGameObject(UASSaveGame::StaticClass()));

	game->SaveSlotName = SaveSlotName;
	game->SaveIndex = 0;

	game = Cast<UASSaveGame>(UGameplayStatics::LoadGameFromSlot(game->SaveSlotName, game->SaveIndex));

	return game;
}

void AASPlayerController::InitSoundMix()
{
	if(!UGameplayStatics::DoesSaveGameExist("GameOptionData_Audio",0)) return;
	UASSaveGame* ld = LoadData("GameOptionData_Audio");
	if (soundMix)
	{
		if(ld)
		{
			for (int i = 0; i < soundMix->SoundClassEffects.Num(); i++)
			{
				FString soundName = soundMix->SoundClassEffects[i].SoundClassObject.GetName();

				if (soundName.Equals(FString("Music")))
				{
					soundMix->SoundClassEffects[i].SoundClassObject->Properties.Volume = ld->soundOption.MusicSoundVolum / 100;
				}
				else if (soundName.Equals(FString("Overall")))
				{
					soundMix->SoundClassEffects[i].SoundClassObject->Properties.Volume = ld->soundOption.VolumeSoundVolum / 100;
				}
				else if (soundName.Equals(FString("SFX")))
				{
					soundMix->SoundClassEffects[i].SoundClassObject->Properties.Volume = ld->soundOption.SFXSoundVolum / 100;
				}
				else if (soundName.Equals(FString("UI")))
				{
					soundMix->SoundClassEffects[i].SoundClassObject->Properties.Volume = ld->soundOption.UISoundVolum / 100;
				}
				else if (soundName.Equals(FString("VoiceChat")))
				{
					soundMix->SoundClassEffects[i].SoundClassObject->Properties.Volume = ld->soundOption.VoiceChatSoundVolum / 100;
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Not Find Sound , Sound Name : %s"), *soundName);
				}
			}
		}
		else
		{
			for(int i = 0; i < soundMix->SoundClassEffects.Num(); i++)
				soundMix->SoundClassEffects[i].SoundClassObject->Properties.Volume = 1.f;
		}
	}
}

void AASPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AASPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}
