// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASHUD.h"

#include "Character/ASPlayerCharacter.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"

AASHUD::AASHUD():
CrossHairOffSet(50.f),
CrossHairHeight(32.f),
CrossHairWidth(32.f),
CrossHairSpreadMax(16.f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AASHUD::DrawHUD()
{
	Super::DrawHUD();
	float sizeX = Canvas->SizeX;
	float sizeY = Canvas->SizeY;

	FVector2D ScreenCenter {sizeX/2,sizeY/2};

	FVector2D CrossHairLocation = FVector2D{ScreenCenter.X,ScreenCenter.Y - CrossHairOffSet};
	
	float w = CrossHairLocation.X - CrossHairWidth;
	float h = CrossHairLocation.Y - CrossHairHeight;
	
	CrossHairSpreadMultiplier = PlayerCharacter->GetCrossHairSpreadMultiplier();
	
	float offset = (CrossHairSpreadMultiplier * CrossHairSpreadMax);
	
	if(UpCrossHair)
	{
		DrawTexture(UpCrossHair,w,h + offset,UpCrossHair->GetSurfaceWidth(),UpCrossHair->GetSurfaceHeight(),
		0.f, 0.f,1.f,1.f);
	}

	if(DownCrossHair)
	{
		DrawTexture(DownCrossHair,w,h - offset,DownCrossHair->GetSurfaceWidth(),DownCrossHair->GetSurfaceHeight(),
		0.f, 0.f,1.f,1.f);
	}

	if(LeftCrossHair)
	{
		DrawTexture(LeftCrossHair,w + offset,h,LeftCrossHair->GetSurfaceWidth(),LeftCrossHair->GetSurfaceHeight(),
		0.f, 0.f,1.f,1.f);
	}

	if(RightCrossHair)
	{
		DrawTexture(RightCrossHair,w - offset,h,RightCrossHair->GetSurfaceWidth(),RightCrossHair->GetSurfaceHeight(),
		0.f, 0.f,1.f,1.f);
	}
}

void AASHUD::BeginPlay()
{
	Super::BeginPlay();

	if(!PlayerCharacter.Get()->IsValidLowLevel())
	{
		PlayerCharacter = Cast<AASPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	}
}

