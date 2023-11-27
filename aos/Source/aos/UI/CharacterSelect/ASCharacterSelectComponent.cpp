// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterSelect/ASCharacterSelectComponent.h"
#include "GameCore/ASGame/ASExperienceStateComponent.h"
#include "GameCore/ASGame/ASGameState.h"
#include "../../../Experimental/ControlFlows/Source/ControlFlows/Public/ControlFlowManager.h"
#include "../../../Experimental/ControlFlows/Source/ControlFlows/Public/ControlFlowNode.h"
#include "../../../Experimental/CommonUI/Source/CommonUI/Public/CommonActivatableWidget.h"
#include "UI/UICore/ASPrimaryGameLayout.h"
#include "GameCore/ASGame/FASGamePlayTags.h"
#include "Data/ASCharacterData.h"

// Sets default values for this component's properties
UASCharacterSelectComponent::UASCharacterSelectComponent(const FObjectInitializer& ObjectInitializer)
 : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UASCharacterSelectComponent::BeginPlay()
{
	Super::BeginPlay();
	
	bShouldShowLoadingScreen = true;

	AASGameState* stateBase = CastChecked<AASGameState>(GetGameStateChecked<AGameStateBase>());
	if(stateBase)
	{
		UASExperienceStateComponent* experienceComponent = stateBase->ExperienceState;
		if (experienceComponent)
			experienceComponent->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &UASCharacterSelectComponent::OnExperienceLoadComplete));
	}
}

void UASCharacterSelectComponent::OnExperienceLoadComplete(const UASExperienceDefinition* experience)
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("FrontendFlow"));
	Flow.QueueStep(TEXT("Wait For User Initialization"), this, &UASCharacterSelectComponent::FlowStep_WaitForUserInitialization);
	Flow.QueueStep(TEXT("Show MainScreen"), this, &UASCharacterSelectComponent::FlowStep_TryShowMainScreen);

	Flow.ExecuteFlow();

	FrontEndFlow = Flow.AsShared();
}

void UASCharacterSelectComponent::FlowStep_WaitForUserInitialization(FControlFlowNodeRef SubFlow)
{	
	LoadSelectCharacters([SubFlow](bool IsComplete)
	{
		if(IsComplete)
			SubFlow->ContinueFlow();
	});
}

void UASCharacterSelectComponent::FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow)
{
	if (UASPrimaryGameLayout* RootLayout = UASPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this))
	{
		check(mainScreenClass);
		constexpr bool bSuspendInputUntilComplete = true;
		RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(FASGameplayTags::Get().Tag_UI_Layer_Menu, bSuspendInputUntilComplete, mainScreenClass,
			[this, SubFlow](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen) {
				switch (State)
				{
				case EAsyncWidgetLayerState::AfterPush:
					bShouldShowLoadingScreen = false;
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

void UASCharacterSelectComponent::OnLoadCharacterComplete(bool IsComplete)
{
	
}

void UASCharacterSelectComponent::LoadSelectCharacters(TFunction<bool> state)
{
	UASAssetManager& assetManager = UASAssetManager::Get();
	FPrimaryAssetId ExperienceId = FPrimaryAssetId(FPrimaryAssetType("ASCharacterData"), FName("DefaultExperience"));
	FSoftObjectPath AssetPath = assetManager.GetPrimaryAssetPath(ExperienceId);
	
	TSubclassOf<UASCharacterData> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	check(AssetClass);
	const TObjectPtr<UASCharacterData> characterData = AssetClass.GetDefaultObject();

	for each (auto var in characterData->InGameCharacterBp)
	{
		FStreamableDelegate StemableDelegate = FStreamableDelegate::CreateLambda(this, &UASCharacterSelectComponent::OnLoadCharacterComplete);
		TSharedPtr<FStreamableHandle> Handle = assetManager.GetStreamableManager().RequestAsyncLoad(var, StemableDelegate);
	}
}

bool UASCharacterSelectComponent::ShouldShowLoadingScreen(FString& OutReason) const
{
	if(bShouldShowLoadingScreen)
	{
		if (FrontEndFlow.IsValid())
		{
			const TOptional<FString> StepDebugName = FrontEndFlow->GetCurrentStepDebugName();
			if (StepDebugName.IsSet())
			{
				OutReason = StepDebugName.GetValue();
			}
		}
		return true;
	}

	return false;
}

void UASCharacterSelectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
