// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/ASGame/ASExperienceStateComponent.h"
#include "Data/ASExperienceDefinition.h"
#include "Data/ExperienceActionSet.h"
#include "../../../Experimental/GameFeatures/Source/GameFeatures/Public/GameFeaturesSubsystemSettings.h"
#include "Engine/StreamableManager.h"
#include "../../../Experimental/GameFeatures/Source/GameFeatures/Public/GameFeaturesSubsystem.h"
#include "ASAssetManager.h"
#include "Engine/AssetManager.h"
#include "aos.h"
#include "../../../Experimental/GameFeatures/Source/GameFeatures/Public/GameFeatureAction.h"

// Sets default values for this component's properties
UASExperienceStateComponent::UASExperienceStateComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UASExperienceStateComponent::ShouldShowLoadingScreen(FString& OutReason) const
{
	if(currentFeartureState != EFeatureLoadState::Loaded)
	{
		OutReason = TEXT("Experience Still loading");
		return true;
	}
	else
		return false;
}

void UASExperienceStateComponent::CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& Delegate)
{
	if(IsExperienceLoaded())
	{
		Delegate.Execute(currentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void UASExperienceStateComponent::ServerSetCurrentExperience(FPrimaryAssetId primaryID, const FString& ExperienceIDSource)
{
	UASAssetManager& Assetmanager = UASAssetManager::Get();

	FSoftObjectPath AssetPath = Assetmanager.GetPrimaryAssetPath(primaryID);
	//TArray<FPrimaryAssetId> OutAssets;
	//Assetmanager.GetPrimaryAssetIdList(TEXT("ASExperienceDefinition"), OutAssets);

	//int32 AccessIdx = 3;
	//FSoftObjectPtr AssetPtr(Assetmanager.GetPrimaryAssetPath(OutAssets[AccessIdx]));
	//// 3. 에셋의 경로를 통해 에셋에 대한 FSoftObjectPtr을 얻어온다.
	//
	//if (AssetPtr.IsPending())
	//{
	//	AssetPtr.LoadSynchronous();
	//	// 4. 에셋을 로딩해온다.
	//}

	TSubclassOf<UASExperienceDefinition> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	check(AssetClass);
	const TObjectPtr<UASExperienceDefinition> Experience = AssetClass.GetDefaultObject();
	check(Experience!=nullptr);
	check(currentExperience == nullptr);

	currentExperience = Experience;

	StartLoad();
}

void UASExperienceStateComponent::StartLoad()
{
	check(currentExperience != nullptr);
	check(currentFeartureState == EFeatureLoadState::Unload);

	currentFeartureState = EFeatureLoadState::Loading;

	UASAssetManager& AssetManager = UASAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;
	TSet<FSoftObjectPath> RawAssetList;

	BundleAssetList.Add(currentExperience->GetPrimaryAssetId());

	for (const TObjectPtr<UExperienceActionSet>& actionSet : currentExperience->ActionSets)
	{
		if(actionSet)
			BundleAssetList.Add(actionSet->GetPrimaryAssetId());
	}

	TArray<FName> BundlesToLoad;

	const ENetMode ownerNetMode = GetOwner()->GetNetMode();
	const bool bLoadClient = GIsEditor || (ownerNetMode != NM_DedicatedServer);
	const bool bLoadServer = GIsEditor || (ownerNetMode != NM_Client);

	if(bLoadServer)
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
	if(bLoadClient)
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);

	const TSharedPtr<FStreamableHandle> BundleLoadHandle = AssetManager.ChangeBundleStateForPrimaryAssets(BundleAssetList.Array(), BundlesToLoad, {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);
	const TSharedPtr<FStreamableHandle> RawLoadHandle = AssetManager.LoadAssetList(RawAssetList.Array(), FStreamableDelegate(),FStreamableManager::AsyncLoadHighPriority);

	TSharedPtr<FStreamableHandle> Handle = nullptr;

	if(BundleLoadHandle.IsValid() && RawLoadHandle.IsValid())
	{
		Handle = AssetManager.GetStreamableManager().CreateCombinedHandle({BundleLoadHandle,RawLoadHandle});
	}
	else
	{
		Handle = BundleLoadHandle.IsValid() ? BundleLoadHandle : RawLoadHandle;
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this,&UASExperienceStateComponent::OnLoadComplete);

	if(!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
		{
			OnAssetsLoadedDelegate.ExecuteIfBound();
		}));
	}

	TSet<FPrimaryAssetId> PreloadAssetList;

	if (PreloadAssetList.Num() > 0)
	{
		AssetManager.ChangeBundleStateForPrimaryAssets(PreloadAssetList.Array(), BundlesToLoad, {});
	}
}

void UASExperienceStateComponent::OnLoadComplete()
{
	check(currentFeartureState == EFeatureLoadState::Loading);
	check(currentExperience != nullptr);

	GameFeaturePluginURLs.Reset();

	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
		{
			for (const FString& PluginName : FeaturePluginList)
			{
				FString PluginURL;
				if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, /*out*/ PluginURL))
				{
					This->GameFeaturePluginURLs.AddUnique(PluginURL);
				}
				else
				{
					ensureMsgf(false, TEXT("OnExperienceLoadComplete failed to find plugin URL from PluginName %s for experience %s - fix data, ignoring for this run"), *PluginName, *Context->GetPrimaryAssetId().ToString());
				}
			}
		};

	CollectGameFeaturePluginURLs(currentExperience, currentExperience->GameFeaturesToEnble);
	for (const TObjectPtr<UExperienceActionSet>& ActionSet : currentExperience->ActionSets)
	{
		if (ActionSet != nullptr)
		{
			CollectGameFeaturePluginURLs(ActionSet, ActionSet->GameFeaturesToEnable);
		}
	}

	// Load and activate the features	
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading > 0)
	{
		currentFeartureState = EFeatureLoadState::LoadingGameFeature;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			//ULyraExperienceManager::NotifyOfPluginActivation(PluginURL);
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &UASExperienceStateComponent::GameFeaturePluginLoadComplete));
		}
	}
	else
	{
		FullLoadComplete();
	}
}

void UASExperienceStateComponent::GameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	NumGameFeaturePluginsLoading--;
	if(NumGameFeaturePluginsLoading == 0)
	{
		FullLoadComplete();
	}
}

void UASExperienceStateComponent::FullLoadComplete()
{
	check(currentFeartureState != EFeatureLoadState::Loaded);
	if (currentFeartureState != EFeatureLoadState::LoadingChaosTestingDelay)
	{
		const float DelaySecs = 0.0f;
		if (DelaySecs > 0.0f)
		{
			FTimerHandle DummyHandle;

			currentFeartureState = EFeatureLoadState::LoadingChaosTestingDelay;
			GetWorld()->GetTimerManager().SetTimer(DummyHandle, this, &ThisClass::FullLoadComplete, DelaySecs, /*bLooping=*/ false);

			return;
		}
	}
	currentFeartureState = EFeatureLoadState::ExecutingActions;
	FGameFeatureActivatingContext context;

	const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());

	if(ExistingWorldContext)
	{
		context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
	}

	//auto ActivateListOfActions = [&context](const TArray<UGameFeatureAction*>& ActionList)
	//	{
	//		for (UGameFeatureAction* Action : ActionList)
	//		{
	//			if (Action != nullptr)
	//			{
	//				//@TODO: The fact that these don't take a world are potentially problematic in client-server PIE
	//				// The current behavior matches systems like gameplay tags where loading and registering apply to the entire process,
	//				// but actually applying the results to actors is restricted to a specific world
	//				Action->OnGameFeatureRegistering();
	//				Action->OnGameFeatureLoading();
	//				Action->OnGameFeatureActivating(context);
	//			}
	//		}
	//	};


	for(UGameFeatureAction* Action : currentExperience->Actions)
	{
		if(Action != nullptr)
		{
			Action->OnGameFeatureRegistering();
			Action->OnGameFeatureLoading();
			Action->OnGameFeatureActivating(context);
		}
	}

	//ActivateListOfActions();
	for(const TObjectPtr<UExperienceActionSet>& actinoSet : currentExperience->ActionSets)
	{
		if(actinoSet != nullptr)
		{
			for (UGameFeatureAction* Action : actinoSet->Actions)
			{
				if (Action != nullptr)
				{
					Action->OnGameFeatureRegistering();
					Action->OnGameFeatureLoading();
					Action->OnGameFeatureActivating(context);
				}
			}
		}
	}

	currentFeartureState = EFeatureLoadState::Loaded;

	OnExperienceLoaded.Broadcast(currentExperience);
	OnExperienceLoaded.Clear();
}

void UASExperienceStateComponent::OnActionDeactivationCompleted()
{
	currentFeartureState = EFeatureLoadState::Unload;
	currentExperience = nullptr;
}

void UASExperienceStateComponent::OnAllActionsDeactivated()
{
	check(IsInGameThread());
	++NumObservedPausers;

	if (NumObservedPausers == NumExpectedPausers)
	{
		OnAllActionsDeactivated();
	}
}

bool UASExperienceStateComponent::IsExperienceLoaded()
{
	return (currentFeartureState == EFeatureLoadState::Loaded) && (currentExperience != nullptr);
}


void UASExperienceStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (const FString& PluginURL : GameFeaturePluginURLs)
	{
		UGameFeaturesSubsystem::Get().DeactivateGameFeaturePlugin(PluginURL);
	}

	if (currentFeartureState == EFeatureLoadState::Loaded)
	{
		currentFeartureState = EFeatureLoadState::Deactivating;

		NumExpectedPausers = INDEX_NONE;
		NumObservedPausers = 0;

		FGameFeatureDeactivatingContext Context(FSimpleDelegate::CreateUObject(this, &UASExperienceStateComponent::OnActionDeactivationCompleted));

		const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
		if (ExistingWorldContext)
		{
			Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
		}

		auto DeactivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
			{
				for (UGameFeatureAction* Action : ActionList)
				{
					if (Action)
					{
						Action->OnGameFeatureDeactivating(Context);
						Action->OnGameFeatureUnregistering();
					}
				}
			};

		DeactivateListOfActions(currentExperience->Actions);
		for (const TObjectPtr<UExperienceActionSet>& ActionSet : currentExperience->ActionSets)
		{
			if (ActionSet != nullptr)
			{
				DeactivateListOfActions(ActionSet->Actions);
			}
		}

		NumExpectedPausers = Context.GetNumPausers();

		//if (NumExpectedPausers > 0)
		//{
		//	UE_LOG(LogLyraExperience, Error, TEXT("Actions that have asynchronous deactivation aren't fully supported yet in Lyra experiences"));
		//}

		if (NumExpectedPausers == NumObservedPausers)
		{
			OnAllActionsDeactivated();
		}
	}
}

