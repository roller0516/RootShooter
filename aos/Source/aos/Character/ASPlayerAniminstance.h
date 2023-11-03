// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ASPlayerAniminstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EOffsetState : uint8
{
	EOS_Aiming UMETA(DisplayName = "Aiming"),
	EOS_Hip UMETA(DisplayName = "Hip"),
	EOS_Reloading UMETA (DisplayName = "Reloading"),
	EOS_InAir UMETA (DisplayName = "InAir"),
	EOS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class AOS_API UASPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UASPlayerAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float deltaTime);
	
	virtual void NativeInitializeAnimation() override;
protected:
	void TurnInPlace();
	void SetOffsetState();
	void Lean(float DeltaTime);
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Movement,meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class AASPlayerCharacter> PlayerCharacter;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category=Movement,meta=(AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category=Movement,meta=(AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Aiming,meta=(AllowPrivateAccess = "true"))
	bool bAiming;
	//-------------------------------------------------------TrunYaw

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;

	float TIPCharacterYaw;

	float TIPCharacterYawLastFrame;
		
	FRotator CharacterRotation;

	FRotator CharacterRotationLastFrame;

	float RotationCurve;

	float RotationCurveLastFrame;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
	float YawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	float AimingPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	bool bReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	EOffsetState OffsetState;
};
