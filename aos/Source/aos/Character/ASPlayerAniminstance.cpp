// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASPlayerAniminstance.h"
#include "ASPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UASPlayerAnimInstance::UASPlayerAnimInstance()
{
	Speed = 0.f;
	bIsInAir = false;
	MovementOffsetYaw = 0.f;
	bAiming = false;
	TIPCharacterYaw = 0.f;
	TIPCharacterYawLastFrame = 0.f;
	LastMovementOffsetYaw = 0.0f;
	RootYawOffset = 0.f;
	AimingPitch = 0.f;
	OffsetState = EOffsetState::EOS_Hip;
	CharacterRotation = FRotator::ZeroRotator;
	CharacterRotationLastFrame = FRotator::ZeroRotator;
}

void UASPlayerAnimInstance::UpdateAnimationProperties(float deltaTime)
{
	if(PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AASPlayerCharacter>(TryGetPawnOwner());
	}

	if(PlayerCharacter)
	{
		FVector Velocity{PlayerCharacter->GetVelocity()};
		Velocity.Z = 0;
		Speed = Velocity.Size();

		//캐릭터가 공중에 있는 지 판단
		bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();
		
		bIsAccelerating = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0;
		FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();

		//진행 하는 방향을 Vector로 변경
		FRotator MovementRotator = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());

		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotator,AimRotation).Yaw;
		
		if(Speed > 0)
			LastMovementOffsetYaw = MovementOffsetYaw;

		bAiming = PlayerCharacter->GetAiming();

		SetOffsetState();
	}
	TurnInPlace();
	Lean(deltaTime);
}

void UASPlayerAnimInstance::SetOffsetState()
{
	if (bReloading)
		OffsetState = EOffsetState::EOS_Reloading;
	else if (bIsInAir)
		OffsetState = EOffsetState::EOS_InAir;
	else if (bAiming)
		OffsetState = EOffsetState::EOS_Aiming;
	else
		OffsetState = EOffsetState::EOS_Hip;
}

void UASPlayerAnimInstance::Lean(float DeltaTime)
{
	if (PlayerCharacter == nullptr)return;
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = PlayerCharacter->GetActorRotation();

	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);

	const float Target = (Delta.Yaw) / DeltaTime;
	const float Interp = FMath::FInterpTo(YawDelta, Target, DeltaTime, 6.0f);

	YawDelta = FMath::Clamp(Interp, -90.f, 90.f);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, FString::Printf(TEXT("YawDelta : %f"), YawDelta));
	if (GEngine) GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, FString::Printf(TEXT("CharacterYaw : %f"), Delta.Yaw));
}

void UASPlayerAnimInstance::NativeInitializeAnimation()
{
	PlayerCharacter = Cast<AASPlayerCharacter>(TryGetPawnOwner());
}

void UASPlayerAnimInstance::TurnInPlace()
{
	if (PlayerCharacter == nullptr) return;

	AimingPitch = PlayerCharacter->GetBaseAimRotation().Pitch;
	bReloading = PlayerCharacter->GetCombatState() == ECombatState::ECS_Reloading;
	if (Speed > 0 || bIsInAir) 
	{
		RootYawOffset = 0;
		TIPCharacterYaw = PlayerCharacter->GetActorRotation().Yaw;
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		RotationCurveLastFrame = 0.f;
		RotationCurve = 0.f;
	}
	else 
	{
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		TIPCharacterYaw = PlayerCharacter->GetActorRotation().Yaw;
		const float TIPYawDelta = TIPCharacterYaw - TIPCharacterYawLastFrame;

		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);

		const float Turning =  GetCurveValue(TEXT("Turning"));

		if (Turning > 0) 
		{
			RotationCurveLastFrame = RotationCurve;
			RotationCurve = GetCurveValue(TEXT("Rotation"));
			const float DeltaRotation = RotationCurve - RotationCurveLastFrame;

			RootYawOffset > 0 ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

			const float ABSRootYawOffset = FMath::Abs(RootYawOffset);
			if (ABSRootYawOffset > 90.f) 
			{
				const float YawExcess = ABSRootYawOffset - 90.0f;
				RootYawOffset > 0 ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
			}
		}
		//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, FString::Printf(TEXT("CharacterYaw : %f"), CharacterYaw));
		//if (GEngine) GEngine->AddOnScreenDebugMessage(2, 1, FColor::Red, FString::Printf(TEXT("RootYawOffset : %f"), RootYawOffset));
		//if (GEngine) GEngine->AddOnScreenDebugMessage(3, 1, FColor::Red, FString::Printf(TEXT("CharacterYawLastFrame : %f"), CharacterYawLastFrame));
	}
}
