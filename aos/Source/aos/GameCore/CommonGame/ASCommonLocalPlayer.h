#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "ASCommonLocalPlayer.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, UASCommonLocalPlayer* localPlayer, APlayerController* playerController);
DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerStateSetDelegate, UASCommonLocalPlayer* LocalPlayer, APlayerState* PlayerState);

UCLASS()
class AOS_API UASCommonLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
public:
	UASCommonLocalPlayer();

	FPlayerControllerSetDelegate onPlayerControllerSet;
	//FDelegateHandle CallAndRegister_onPlayerControllerSet(FPlayerControllerSetDelegate::FDelegate delegate);
	FPlayerStateSetDelegate OnPlayerStateSet;
};
