// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonInputBaseTypes.h"
#include "HAL/Platform.h"
#include "Internationalization/Text.h"
#include "UObject/UObjectGlobals.h"
#include "ASButtonBase.generated.h"

class FText;

/**
 * 
 */
UCLASS(Abstract,BlueprintType,Blueprintable)
class AOS_API UASButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText& Intext);

protected:
	// UUserWidget interface
	virtual void NativePreConstruct() override;
	// End of UUserWidget interface

	// UCommonButtonBase interface
	virtual void UpdateInputActionWidget() override;
	virtual void OnInputMethodChanged(ECommonInputType CurrentInputType) override;
	// End of UCommonButtonBase interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonStyle();
private:
	void RefreshButtonText();

private:
	UPROPERTY(EditAnywhere, Category="Button", meta=(EditCondition ="bOverride_ButtonText" ))
	FText ButtonText;

	UPROPERTY(EditAnywhere,Category = "Button", meta = (InlineEditConditionToggle))
	uint8 bOverride_ButtonText : 1;
};
