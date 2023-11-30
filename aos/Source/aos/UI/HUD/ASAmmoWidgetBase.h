// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ASAmmoWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class AOS_API UASAmmoWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UASAmmoWidgetBase(const FObjectInitializer& InitObejct);

	void InitWeaponAmmonCount();

private:
	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UText> ammonCountText;


};
