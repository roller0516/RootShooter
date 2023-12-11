// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyAnimInstance.h"
#include "Character/ASEnemy.h"

void UEnemyAnimInstance::UpdateAnimationProperties(float deltaTime)
{
	if (Enemy == nullptr)
	{
		Enemy = Cast<AASEnemy>(TryGetPawnOwner());
	}

	if (Enemy)
	{
		FVector Velocity{ Enemy->GetVelocity() };
		Velocity.Z = 0.f;
		LeanAngle = Velocity.Size();
	}
}
