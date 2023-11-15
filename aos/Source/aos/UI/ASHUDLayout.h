// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UICore/ASActivatableWidget.h"
#include "ASHUDLayout.generated.h"

/**
 * 
 */

class UCommonActivatableWidget;

UCLASS()
class AOS_API UASHUDLayout : public UASActivatableWidget
{
	GENERATED_BODY()
public:
	UASHUDLayout(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
protected:
	//ESC ´©¸£¸é
	void EscapeActionHandle();

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};
