// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FrontEnd/ASFrontendComponent.h"
#include "ControlFlowNode.h"
#include "ControlFlowManager.h"
#include "aos.h"
#include "GameCore/ASGame/ASExperienceStateComponent.h"
#include "UI/UICore/ASPrimaryGameLayout.h"
#include "NativeGameplayTags.h"
#include "GameCore/ASGame/FASGamePlayTags.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/CommonActivatableWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Data/ASExperienceDefinition.h"

//UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
//UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");
//UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");

UASFrontendComponent::UASFrontendComponent(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

bool UASFrontendComponent::ShouldShowLoadingScreen(FString& OutReason) const
{
	if(bShouldShowLoadingScreen)
	{
		if(FrontEndFlow.IsValid())
		{	
			const TOptional<FString> StepDebugName = FrontEndFlow->GetCurrentStepDebugName();
			if(StepDebugName.IsSet())
			{
				OutReason = StepDebugName.GetValue();
			}
		}

		return true;
	}

	return false;
}

void UASFrontendComponent::BeginPlay()
{
	Super::BeginPlay();

	bShouldShowLoadingScreen = true;

	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	UASExperienceStateComponent* experienceComponent = GameState->FindComponentByClass<UASExperienceStateComponent>();

	experienceComponent->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this,&UASFrontendComponent::OnExperiencedLoaded));
}

void UASFrontendComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UASFrontendComponent::OnExperiencedLoaded(const UASExperienceDefinition* Experience)
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("FrontendFlow"))
		.QueueStep(TEXT("Wait For User Initialization"), this, &UASFrontendComponent::FlowStep_WaitForUserInitialization)
		.QueueStep(TEXT("Try Show Press Start Screen"), this, &UASFrontendComponent::FlowStep_TryShowPressStartScreen)
		.QueueStep(TEXT("Try Join Requested Session"), this, &UASFrontendComponent::FlowStep_TryJoinRequestedSession)
		.QueueStep(TEXT("Try Show Main Screen"), this, &UASFrontendComponent::FlowStep_TryShowMainScreen);

	experience = const_cast<UASExperienceDefinition*>(Experience);

	Flow.ExecuteFlow();

	FrontEndFlow = Flow.AsShared();
}

void UASFrontendComponent::FlowStep_WaitForUserInitialization(FControlFlowNodeRef SubFlow)
{
	SubFlow->ContinueFlow();
}

void UASFrontendComponent::FlowStep_TryShowPressStartScreen(FControlFlowNodeRef SubFlow)
{
	SubFlow->ContinueFlow();
}

void UASFrontendComponent::FlowStep_TryJoinRequestedSession(FControlFlowNodeRef SubFlow)
{
	SubFlow->ContinueFlow();
}

void UASFrontendComponent::FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow)
{
	if (UASPrimaryGameLayout* RootLayout = UASPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this))
	{
		constexpr bool bSuspendInputUntilComplete = true;
		RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(FASGameplayTags::Get().Tag_UI_Layer_Menu, bSuspendInputUntilComplete, mainScreenClass,
			[this, SubFlow](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen) {
				switch (State)
				{
				case EAsyncWidgetLayerState::AfterPush:
					bShouldShowLoadingScreen = false;

					if (experience->bgSound)
						UGameplayStatics::PlaySound2D(GetWorld(), experience->bgSound);

					SubFlow->ContinueFlow();
					return;
				case EAsyncWidgetLayerState::Canceled:
					bShouldShowLoadingScreen = false;
					SubFlow->ContinueFlow();
					return;
				}
			});
	}
}
