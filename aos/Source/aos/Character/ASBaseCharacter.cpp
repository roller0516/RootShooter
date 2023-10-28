// Fill out your copyright notice in the Description page of Project Settings.


#include "ASBaseCharacter.h"

// Sets default values
AASBaseCharacter::AASBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void AASBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AASBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

