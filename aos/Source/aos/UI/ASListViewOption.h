// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASListViewOption.generated.h"

class UListView;

UCLASS()
class AOS_API UASListViewOption : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* EntriesVerticalBox;

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<class UBUIInventoryEntry> EntryClass;
	
};
