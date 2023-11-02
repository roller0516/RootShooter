// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ASPlayerAniminstance.generated.h"

/**
 * 
 */
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

	float CharacterYaw;

	float CharacterYawLastFrame;

	float RotationCurve;

	float RotationCurveLastFrame;
};
