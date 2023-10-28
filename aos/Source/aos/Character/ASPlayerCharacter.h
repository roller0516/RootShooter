// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ASBaseCharacter.h"
#include "ASPlayerCharacter.generated.h"

UCLASS()
class AOS_API AASPlayerCharacter : public AASBaseCharacter
{
	GENERATED_BODY()

public:
	AASPlayerCharacter();
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;
	
	void CharacterMove(const FInputActionValue& Value);
	void CharacterLook(const FInputActionValue& Value);
	
private :
	//---------------------------------------------------------Camera
	UPROPERTY()
	class UCameraComponent* Camera;
	UPROPERTY()
	class USpringArmComponent* CameraBoom;

	//---------------------------------------------------------Input
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* defaultMappingContext;
	 
	UPROPERTY(EditAnywhere)
	class UInputAction* IAMove;

	UPROPERTY(EditAnywhere)
	class UInputAction* IALook;

	UPROPERTY(EditAnywhere)
	class UInputAction* IAJump;
	
	UPROPERTY(EditAnywhere)
	class UInputAction* IAAttack;

	UPROPERTY(EditAnywhere)
	class UInputAction* IASkill1;

	UPROPERTY(EditAnywhere)
	class UInputAction* IASkill2;
};
