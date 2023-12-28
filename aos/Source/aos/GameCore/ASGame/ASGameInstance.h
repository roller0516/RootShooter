// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/CommonGame/ASCommonGameInstance.h"
#include "EnumTypes.h"
#include "ASGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AOS_API UASGameInstance : public UASCommonGameInstance
{
	GENERATED_BODY()
public:
	UASGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Init()override;
	virtual void Shutdown() override;

public:
	UPROPERTY()
	class UASItemPrimaryData* ItemData;

	UPROPERTY(BlueprintReadOnly)
	class UASGameOptionData* GameOpitionData;

	//UPROPERTY()
	//class USoundClass* BGSound;

	FORCEINLINE CharID GetCharID() { return selectCharID; }
	FORCEINLINE void SetCharID(CharID charid) { selectCharID = charid; }
private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	CharID selectCharID;
};
