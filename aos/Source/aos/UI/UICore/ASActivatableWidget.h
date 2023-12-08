// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ASActivatableWidget.generated.h"

/**
 * Ȱ��ȭ�Ǹ� ���ϴ� �Է� ������ �ڵ����� �����ϴ� Ȱ��ȭ ������ ����
 */

UENUM()
enum class EWidgetInputMode : int8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

UCLASS()
class AOS_API UASActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UASActivatableWidget(const FObjectInitializer& ObjectInitializer);

	// UCommonActivatableWidget InterFace
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	// UCommonActivatableWidget InterFace
private:
	//void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const;

protected:
	virtual void EscapeActionHandle();
protected:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EWidgetInputMode InputMode = EWidgetInputMode::Default;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode MouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
