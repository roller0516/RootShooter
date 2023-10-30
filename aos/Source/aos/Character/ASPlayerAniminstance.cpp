// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASPlayerAniminstance.h"
#include "ASPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	}
}

void UASPlayerAnimInstance::NativeInitializeAnimation()
{
	PlayerCharacter = Cast<AASPlayerCharacter>(TryGetPawnOwner());
}
