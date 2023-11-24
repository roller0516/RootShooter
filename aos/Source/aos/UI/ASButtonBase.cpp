// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASButtonBase.h"
#include "CommonActionWidget.h"

void UASButtonBase::SetButtonText(const FText& Intext)
{
	bOverride_ButtonText = Intext.IsEmpty();
	ButtonText = Intext;
	RefreshButtonText();
}

void UASButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateButtonStyle();
	RefreshButtonText();
}

void UASButtonBase::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();

	UpdateButtonStyle();
	RefreshButtonText();
}

void UASButtonBase::RefreshButtonText()
{
	if(bOverride_ButtonText || ButtonText.IsEmpty())
	{
		if(InputActionWidget)
		{
			const FText ActionDisplayText = InputActionWidget->GetDisplayText();
			if(!ActionDisplayText.IsEmpty())
			{
				UpdateButtonText(ActionDisplayText);
				return;
			}
		}
	}
	UpdateButtonText(ButtonText);
}

void UASButtonBase::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);

	UpdateButtonStyle();
}