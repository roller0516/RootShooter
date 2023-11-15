// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ASGameUIPolicy.generated.h"

/**
 * 
 */

class UASPrimaryGameLayout;
class ULocalPlayer;
class UASCommonLocalPlayer;
class UGameUIManagerSubsystem;

USTRUCT()
struct FRootViewPortLayoutInfo
{
	GENERATED_BODY()
	//Transient : UPROPERTY를 통해 직렬화를 하여 데이터를 가지고 있게 되는데(메모리 공간차지) 
	//자주 변하는 속성은 직렬화에서 제외 해주기위함.
public:

	FRootViewPortLayoutInfo() {}
	FRootViewPortLayoutInfo(ULocalPlayer* _localPlayer, UASPrimaryGameLayout* _primaryGameLayout, bool _bIsInViewport)
		: localPlayer(_localPlayer),
		primaryGameLayout(_primaryGameLayout),
		bAddedViewPort(_bIsInViewport)
	{
	}

	UPROPERTY(Transient)
	ULocalPlayer* localPlayer;

	UPROPERTY(Transient)
	UASPrimaryGameLayout* primaryGameLayout;

	UPROPERTY(Transient)
	bool bAddedViewPort;

	bool operator==(const ULocalPlayer* _localplayer)const { return localPlayer == _localplayer; }
	//bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return localPlayer == OtherLocalPlayer; }
};

UCLASS(Abstract, Blueprintable ,Within = ASGameUIManagerSubSystem)
class AOS_API UASGameUIPolicy : public UObject
{
	GENERATED_BODY()
public:
	template<typename GameUIPolicyClass = UASGameUIPolicy>
	static GameUIPolicyClass* GetGameUIPolicyAs(const UObject* WorldContextObject)
	{
		return Cast<UASGameUIPolicy>();
	}

	static UASGameUIPolicy* GetGameUIPolicy(const UObject* Worldcontext);

	void NotifyAddPlayer(UASCommonLocalPlayer* localPlayer);
	void NotifyRemovePlayer(UASCommonLocalPlayer* localPlayer);
	void NotifyDestroyPlayer(UASCommonLocalPlayer* localPlayer);

	UASPrimaryGameLayout* GetRootLayout(const UASCommonLocalPlayer* LocalPlayer) const;
protected:
	void AddToViewPort(UASCommonLocalPlayer* localPlayer,UASPrimaryGameLayout* layout);
	void RemoveLayoutFromViewport(UASCommonLocalPlayer* localPlayer, UASPrimaryGameLayout* layout);
	void CreateLayoutWidget(UASCommonLocalPlayer* localPlayer);

	TSubclassOf<UASPrimaryGameLayout> GetLayoutWidgetClass(UASCommonLocalPlayer* localPlayer);
protected:

	UPROPERTY(Transient)
	TArray<FRootViewPortLayoutInfo> ViewportLayoutInfos;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UASPrimaryGameLayout> LayoutClass;
};
