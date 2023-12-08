// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UICore/ASActivatableWidget.h"
#include "ASInventoryView.generated.h"

/**
 * 
 */

 class UUserWidget;

UCLASS()
class AOS_API UASInventoryView : public UASActivatableWidget
{
	GENERATED_BODY()
public:

	UASInventoryView(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void ShowEquipView();

	UFUNCTION(BlueprintCallable)
	void ShowInventoryView();
private:
	UPROPERTY(meta = (BindWidget , AllowPrivateAccess = "true"))
	class UCanvasPanel*	equipPanel;

	UPROPERTY(meta = (BindWidget , AllowPrivateAccess = "true"))
	class UCanvasPanel* inventoryPanel;

	virtual void NativeOnInitialized() override;

	virtual void EscapeActionHandle() override;

	bool isShowEquip;
};
