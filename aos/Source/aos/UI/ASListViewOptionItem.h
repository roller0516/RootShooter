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
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OptionTitle;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OptionNames;
};
