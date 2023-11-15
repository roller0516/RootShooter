// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/CommonLoadingScreen/ASLoadingScreenManager.h"
#include "UObject/UObjectGlobals.h"
#include "Interface/Command/ILoadingProcessInterFace.h"
#include "aos.h"
#include "GameFramework/GameStateBase.h"
#include "FLoadingScreenInputPreProcessor.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "CommonLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"
#include "ShaderPipelineCache.h"

//#include "FLoadingScreenInputPreProcessor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ASLoadingScreenManager) // 헤더 인라인 컴파일 시간 단축

bool IILoadingProcessInterFace::ShouldShowLoadingScreen(UObject* TestObject, FString& OutReson)
{
	if (TestObject != nullptr)
	{
		if (IILoadingProcessInterFace* LoadObserve = Cast<IILoadingProcessInterFace>(TestObject))
		{
			FString ObserverReson;
			if (LoadObserve->ShouldShowLoadingScreen(ObserverReson))
			{
				if (ensureMsgf(!ObserverReson.IsEmpty(), TEXT("")))
				{
					OutReson = ObserverReson;
				}

				return true;
			}
		}
	}
	return false;
}

//class FLoadingScreenInputPreProcessor : public IInputProcessor
//{
//public:
//	FLoadingScreenInputPreProcessor() { }
//	virtual ~FLoadingScreenInputPreProcessor() { }
//
//	bool CanEatInput() const
//	{
//		return !GIsEditor;
//	}
//
//	//~IInputProcess interface
//	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override { }
//
//	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override { return CanEatInput(); }
//	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override { return CanEatInput(); }
//	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override { return CanEatInput(); }
//	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
//	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
//	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
//	virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
//	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override { return CanEatInput(); }
//	virtual bool HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent) override { return CanEatInput(); }
//	//~End of IInputProcess interface
//};


void UASLoadingScreenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this,&ThisClass::HandlePreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&ThisClass::HandlePostLoadMap);

	const UGameInstance* LocalGameInstance = GetGameInstance();
	check(LocalGameInstance);
}

void UASLoadingScreenManager::Deinitialize()
{

}

bool UASLoadingScreenManager::ShouldCreateSubsystem(UObject* Outer) const
{
	//const UGameInstance* GameInstance = CastChecked<UGameInstance>(Outer);
	//const bool bIsServerWorld = GameInstance->IsDedicatedServerInstance();
	return true;
}

void UASLoadingScreenManager::HandlePreLoadMap(const FWorldContext& WorldContext, const FString& MapName)
{
	const UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		if (UGameViewportClient* ViewportClient = GameInstance->GetGameViewportClient())
		{
			UE_LOG(LogLoadScreenManager, Log, TEXT("%s"), *MapName);
		}
	}
}

void UASLoadingScreenManager::HandlePostLoadMap(UWorld* LoadedWorld)
{
	if(LoadedWorld && LoadedWorld->GetGameInstance())
	{
		UE_LOG(LogLoadScreenManager,Log,TEXT("%s"),*LoadedWorld->GetName());
	}
}

void UASLoadingScreenManager::Tick(float DeltaTime)
{
	
}

TStatId UASLoadingScreenManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UASLoadingScreenManager, STATGROUP_Tickables);
}

bool UASLoadingScreenManager::CheckForAnyNeedToShowLoadingScreen()
{
	DebugLogMsg = FString("Showing || Hiding LoadingScreen is Unknown!");
	
	const UGameInstance* GameInstance = GetGameInstance();
	
	const FWorldContext* Context = GameInstance->GetWorldContext();
	if(Context == nullptr)
	{
		//Debug
		DebugLogMsg = FString("GameInstance has a null World Context");
	
		return true;
	}
	
	UWorld* World = Context->World();
	if(World == nullptr)
	{
		//Debug
		DebugLogMsg = FString("World is Null");
		return true;
	}
	
	AGameStateBase* GameState = World->GetGameState<AGameStateBase>();
	if (GameState == nullptr)
	{
		//Debug
		DebugLogMsg = FString("GameState is Null");
		return true;
	}
	
	if(bCurrentlyInLoadMap)
	{
		DebugLogMsg = FString("bCurrentlyInLoadMap is True");
		return true;
	}
	
	if(Context->TravelURL.IsEmpty())
	{
		DebugLogMsg = FString("Context TravleURL is Empty");
		return true;
	}
	
	if(World->IsInSeamlessTravel())
	{
		DebugLogMsg = FString("Seamless Travel");
		return true;
	}
	
	if(IILoadingProcessInterFace::ShouldShowLoadingScreen(GameState, DebugLogMsg))
	{
		return true;
	}
	
	for(UActorComponent* actorComponent : GameState->GetComponents())
	{
		if(IILoadingProcessInterFace::ShouldShowLoadingScreen(actorComponent,DebugLogMsg))
		{
			return true;
		}
	}
	
	return false;
}

bool UASLoadingScreenManager::ShouldShowLoadingScreen()
{
	bool bNeedToShowLoadingScreen = CheckForAnyNeedToShowLoadingScreen();
	bool bWantToForceShowLoadingScreen = false;
	if (bNeedToShowLoadingScreen)
	{
		TimeLoadingScreenLastDismissed = -1;
	}
	else
	{
		const double CurrentTime = FPlatformTime::Seconds();
		const bool bCanHoldLoadingScreen = GIsEditor;
		const double HoldLoadingScreenAdditionalSecs = 2.0f;

		if (TimeLoadingScreenLastDismissed < 0.0)
		{
			TimeLoadingScreenLastDismissed = CurrentTime;
		}
		const double TimeSinceScreenDismissed = CurrentTime - TimeLoadingScreenLastDismissed;
		if ((HoldLoadingScreenAdditionalSecs > 0.0) && TimeSinceScreenDismissed < HoldLoadingScreenAdditionalSecs)
		{
			UGameViewportClient* gameviewPort = GetGameInstance()->GetGameViewportClient();
			gameviewPort->bDisableWorldRendering = false;

			DebugLogMsg = FString::Printf(TEXT("Keeping loading screen up for an additional %.2f seconds to allow texture streaming"), HoldLoadingScreenAdditionalSecs);
			bWantToForceShowLoadingScreen = true;
		}
	}
	return 	bNeedToShowLoadingScreen || bWantToForceShowLoadingScreen;
}

void UASLoadingScreenManager::RemoveWidgetFromViewport()
{
	UGameInstance* LocalGameInstance = GetGameInstance();
	if (LoadingScreenWidget.IsValid())
	{
		if (UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient())
		{
			GameViewportClient->RemoveViewportWidgetContent(LoadingScreenWidget.ToSharedRef());
		}
		LoadingScreenWidget.Reset();
	}
}

void UASLoadingScreenManager::StartBlockingInput() 
{
	if (!InputPreProcessor.IsValid())
	{
		InputPreProcessor = MakeShareable<FLoadingScreenInputPreProcessor>(new FLoadingScreenInputPreProcessor());

		//IInputProcessor* ip = Cast<IInputProcessor>(InputPreProcessor);
		FSlateApplication& SlateApplication = FSlateApplication::Get();
		SlateApplication.RegisterInputPreProcessor(InputPreProcessor, 0);
	}
}

void UASLoadingScreenManager::ChangePerformanceSettings(bool bEnabingLoadingScreen)
{
	UGameInstance* gameInstance = GetGameInstance();
	UGameViewportClient* viewportClient = gameInstance->GetGameViewportClient();

	FShaderPipelineCache::SetBatchMode(bEnabingLoadingScreen ? FShaderPipelineCache::BatchMode::Fast : FShaderPipelineCache::BatchMode::Background);

	viewportClient->bDisableWorldRendering = bEnabingLoadingScreen;

	if(UWorld* viewportWorld = viewportClient->GetWorld())
	{
		if(AWorldSettings* worldsettings = viewportWorld->GetWorldSettings(false,false))
		{
			worldsettings->bHighPriorityLoadingLocal = bEnabingLoadingScreen;
		}
	}
}

void UASLoadingScreenManager::HideLoadingScreen()
{
	if (!bCurrentlyShowingLoadingScreen)
	{
		return;
	}

	StopBlockingInput();

	GEngine->ForceGarbageCollection(true);

	RemoveWidgetFromViewport();

	ChangePerformanceSettings(/*bEnableLoadingScreen=*/ false);

	// Let observers know that the loading screen is done
	OnLoadingScreenVisibilityChanged.Broadcast(/*bIsVisible=*/ false);

	const double LoadingScreenDuration = FPlatformTime::Seconds() - TimeLoadingScreen;
	UE_LOG(LogLoadScreenManager, Log, TEXT("LoadingScreen was visible for %.2fs"), LoadingScreenDuration);

	bCurrentlyShowingLoadingScreen = false;
}

void UASLoadingScreenManager::StopBlockingInput()
{
	if(!InputPreProcessor.IsValid())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputPreProcessor);
		InputPreProcessor.Reset();
	}
}

void UASLoadingScreenManager::ShowLoadingScreen()
{
	if (bCurrentlyShowingLoadingScreen)
	{
		return;
	}

	const UCommonLoadingScreenSettings* Settings = GetDefault<UCommonLoadingScreenSettings>();

	TimeLoadingScreen = FPlatformTime::Seconds();

	bCurrentlyShowingLoadingScreen = true;

	UE_LOG(LogLoadScreenManager, Log, TEXT("%s"), *DebugLogMsg);

	UGameInstance* gameInstance = GetGameInstance();

	StartBlockingInput();

	OnLoadingScreenVisibilityChanged.Broadcast(true);

	TSubclassOf<UUserWidget> LoadingScreenWidgetClass = Settings->LoadingScreenWidget.TryLoadClass<UUserWidget>();
	//showLoading Screen

	if(UUserWidget* userWidget = UUserWidget::CreateWidgetInstance(*gameInstance, LoadingScreenWidgetClass,FName(TEXT(""))))
	{
		LoadingScreenWidget = userWidget->TakeWidget();
	}

	UGameViewportClient* GameViewportClient = gameInstance->GetGameViewportClient();
	GameViewportClient->AddViewportWidgetContent(LoadingScreenWidget.ToSharedRef(), Settings->zOrder);

	ChangePerformanceSettings(true);
}
