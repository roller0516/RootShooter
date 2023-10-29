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

	// todo:: roller 일단 기능 구현을 먼저 해보자.
	void FireWeapon();
private :
	//---------------------------------------------------------Camera
	UPROPERTY()
	class UCameraComponent* Camera;
	UPROPERTY()
	class USpringArmComponent* CameraBoom;

	//---------------------------------------------------------Input
	UPROPERTY(EditAnywhere,Category = Input)
	class UInputMappingContext* defaultMappingContext;
	 
	UPROPERTY(EditAnywhere,Category = Input)
	class UInputAction* IAMove;

	UPROPERTY(EditAnywhere,Category = Input)
	class UInputAction* IALook;

	UPROPERTY(EditAnywhere,Category = Input)
	class UInputAction* IAJump;
	
	UPROPERTY(EditAnywhere,Category = Input)
	class UInputAction* IAAttack;

	UPROPERTY(EditAnywhere,Category = Input)
	class UInputAction* IASkill1;

	UPROPERTY(EditAnywhere,Category = Input)
	class UInputAction* IASkill2;

	//--------------------------------------------------------particle
	UPROPERTY(EditAnywhere,Category = Particle)
	class UParticleSystem* MuzzleFlash;

	
	//--------------------------------------------------------Montage
	UPROPERTY(EditAnywhere,Category = Motage)
	class UAnimMontage* HipFireMontage;
};
