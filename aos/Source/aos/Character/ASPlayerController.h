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


};
