// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASBaseCharacter.generated.h"

class IIBehaviorCommand;

UCLASS(Abstract,config = Input)
class AOS_API AASBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AASBaseCharacter();

protected:
	virtual void BeginPlay() override;
	//virtual void SetBehaviorCommand(IIBehaviorCommand command);
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
