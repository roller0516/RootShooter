// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ASPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AOS_API AASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AASPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface

	//~ Begin APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~ End APlayerController interface

	virtual void SetPlayer(UPlayer* InPlayer) override;

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable)
	void SaveOptionControllData();

	UFUNCTION(BlueprintCallable)
	void SaveOptionAudioData(float volume,float UI, float VoiceChat,float SFX,float Music);

	class UASSaveGame* SaveGamedata;

	UFUNCTION(BlueprintCallable)
	class UASSaveGame* LoadData(FString SaveSlotName);

	void InitSoundMix();
protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = "true"))
	class USoundMix* soundMix;
};
