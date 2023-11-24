// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UICore/ASActivatableWidget.h"
#include "CommonInputModeTypes.h"
#include "Editor/WidgetCompilerLog.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/Input/CommonUIActionRouterBase.h"

#define LOCTEXT_NAMESPACE "Aos"

UASActivatableWidget::UASActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//UE_LOG(LogTemp,Log,TEXT("UASActivatableWidget construct"));
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

//void UASActivatableWidget::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const
//{
//	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);
//
//	if (!GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UASActivatableWidget, BP_GetDesiredFocusTarget)))
//	{
//		if (GetParentNativeClass(GetClass()) == UASActivatableWidget::StaticClass())
//		{
//			CompileLog.Warning(LOCTEXT("ValidateGetDesiredFocusTarget_Warning", "GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen."));
//		}
//		else
//		{
//			//TODO - Note for now, because we can't guarantee it isn't implemented in a native subclass of this one.
//			CompileLog.Note(LOCTEXT("ValidateGetDesiredFocusTarget_Note", "GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen.  If it was implemented in the native base class you can ignore this message."));
//		}
//	}
//}