// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ASReticleWigetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AOS_API UASReticleWigetBase : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UASReticleWigetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponInit();

	//UFUNCTION(BlueprintCallable)
	//void InitFromWeapon();
	
};
