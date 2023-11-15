// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UICore/ASActivatableWidget.h"
#include "CommonInputModeTypes.h"
#include "Editor/WidgetCompilerLog.h"

UASActivatableWidget::UASActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UASActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputMode)
	{
	case EWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All,MouseCaptureMode);
	case EWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game,MouseCaptureMode);
	case EWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu,EMouseCaptureMode::NoCapture);
	case EWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}