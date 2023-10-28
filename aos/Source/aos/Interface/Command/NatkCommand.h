// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/IBehaviorCommand.h"
#include "UObject/NoExportTypes.h"
#include "NatkCommand.generated.h"

/**
 * 
 */
UCLASS()
class AOS_API UNatkCommand : public UObject , public IIBehaviorCommand
{
	GENERATED_BODY()

public:
	virtual void Excute(TObjectPtr<AActor> Receiver) override;
	
};
