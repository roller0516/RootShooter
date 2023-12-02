// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASReticleWigetBase.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ASPlayerCharacter.h"
#include "Components/Widget.h"
#include "Components/Image.h"

UASReticleWigetBase::UASReticleWigetBase(const FObjectInitializer& ObjectInitializer)
 : Super(ObjectInitializer),
	CrossHairOffSet(50.f),
	CrossHairHeight(32.f),
	CrossHairWidth(32.f),
	CrossHairSpreadMax(16.f)
{

}

//void UASReticleWigetBase::DrawTexture(UTexture* Texture, float ScreenX, float ScreenY, float ScreenW, float ScreenH, float TextureU, 
//float TextureV, float TextureUWidth, float TextureVHeight, FLinearColor Color, EBlendMode BlendMode, float Scale, bool bScalePosition, float Rotation, FVector2D RotPivot)
//{
//	FCanvasTileItem TileItem(FVector2D(ScreenX, ScreenY), Texture->GetResource(), FVector2D(ScreenW, ScreenH) * Scale, FVector2D(TextureU, TextureV), FVector2D(TextureU + TextureUWidth, TextureV + TextureVHeight), Color);
//	TileItem.Rotation = FRotator(0, Rotation, 0);
//	TileItem.PivotPoint = RotPivot;
//	if (bScalePosition)
//	{
//		TileItem.Position *= Scale;
//	}
//	TileItem.BlendMode = FCanvas::BlendToSimpleElementBlend(BlendMode);
//	Canvas->DrawItem(TileItem);
//}

void UASReticleWigetBase::DrawReticle()
{
	//Super::DrawHUD();
	//AHUD* hud = ->GetHUD();
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	//int32 x,y;
	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(x, y);
	float sizeX = ViewportSize.X;
	float sizeY = ViewportSize.Y;

	FVector2D ScreenCenter{ sizeX / 2,sizeY / 2 };

	FVector2D CrossHairLocation = FVector2D{ ScreenCenter.X,ScreenCenter.Y - CrossHairOffSet };

	float w = CrossHairLocation.X - CrossHairWidth;
	float h = CrossHairLocation.Y - CrossHairHeight;

	PlayerCharacter = Cast<AASPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	CrossHairSpreadMultiplier = PlayerCharacter->GetCrossHairSpreadMultiplier();
	float offset = (CrossHairSpreadMultiplier * CrossHairSpreadMax);

	FWidgetTransform Tr;

	if (UpCrossHair)
	{
		UpCrossHairValue.X = w;
		UpCrossHairValue.Y = h + offset;
		Tr.Translation = UpCrossHairValue;
		UpCrossHair->SetRenderTransform(Tr);
			//DrawTexture(UpCrossHair, w, h + offset, UpCrossHair->GetSurfaceWidth(), UpCrossHair->GetSurfaceHeight(),
			//	0.f, 0.f, 1.f, 1.f);
	}

	if (DownCrossHair)
	{
		BottomCrossHairValue.X = w;
		BottomCrossHairValue.Y = h - offset;
		Tr.Translation = BottomCrossHairValue;
		DownCrossHair->SetRenderTransform(Tr);
		//DrawTexture(DownCrossHair, w, h - offset, DownCrossHair->GetSurfaceWidth(), DownCrossHair->GetSurfaceHeight(),
		//	0.f, 0.f, 1.f, 1.f);
	}


	if (LeftCrossHair)
	{
		LeftCrossHairValue.X = w + offset;
		LeftCrossHairValue.Y = h;
		Tr.Translation = LeftCrossHairValue;
		LeftCrossHair->SetRenderTransform(Tr);
		//DrawTexture(LeftCrossHair, w + offset, h, LeftCrossHair->GetSurfaceWidth(), LeftCrossHair->GetSurfaceHeight(),
		//	0.f, 0.f, 1.f, 1.f);
	}

	if (RightCrossHair)
	{
		RightCrossHairValue.X = w - offset;
		RightCrossHairValue.Y = h;
		Tr.Translation = RightCrossHairValue;
		RightCrossHair->SetRenderTransform(Tr);
		//DrawTexture(RightCrossHair, w - offset, h, RightCrossHair->GetSurfaceWidth(), RightCrossHair->GetSurfaceHeight(),
		//	0.f, 0.f, 1.f, 1.f);
	}


}

void UASReticleWigetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	DrawReticle();
}