// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ASAmmoWidgetBase.generated.h"

/**
 * 
 */
class UTextBlock;
class UProgressBar;
//이름 수정 예정.
UCLASS()
class AOS_API UASAmmoWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UASAmmoWidgetBase(const FObjectInitializer& InitObejct);

	void InitWeaponAmmonCount(class ASWeapon* weapon);
	void OnChangeHpProgressBar(float hpRate);
	void OnChangeStaminaProgressBar(float staminaRate);
private:
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ammonCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HpProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaProgressBar;
};
