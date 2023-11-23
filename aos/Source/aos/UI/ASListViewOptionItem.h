// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ASListViewOptionItem.generated.h"

class UTextBlock;

UCLASS()
class AOS_API UASListViewOptionItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *NameBoxText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock *OptionText;
};
