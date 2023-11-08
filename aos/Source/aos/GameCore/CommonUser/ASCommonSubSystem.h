// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NativeGameplayTags.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"

#include "ASCommonSubSystem.generated.h"

/**
 * 
 */
 //struct
struct AOS_API FCommonUserTags
{
	static FNativeGameplayTag SystemMessage_Error;
};


UCLASS()
class AOS_API UASCommonSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UASCommonSubSystem(){}
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize()override;

};
