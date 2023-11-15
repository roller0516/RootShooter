// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.h"
#include "Engine/GameInstance.h"
#include "ASCommonGameInstance.generated.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogCommonGame, Log, All);
/**
 * 
 */
 class FText;
 struct FGameplayTag;

UCLASS(Abstract, Config = Game)
class AOS_API UASCommonGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UASCommonGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	virtual void HandleSystemMessage(FGameplayTag MessageType, FText Title, FText Message);

	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;
	virtual void Init() override;
	virtual void ReturnToMainMenu() override;
private:
	//메인 플레이어.
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
};
