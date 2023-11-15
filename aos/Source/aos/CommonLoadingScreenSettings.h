#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "CommonLoadingScreenSettings.generated.h"

UCLASS(Config = Game)
class AOS_API UCommonLoadingScreenSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()
public:
	UPROPERTY(Config,EditAnywhere,Category = Display,meta = (MetaClass = "/Script/UMG.UserWidget"))
	FSoftClassPath LoadingScreenWidget;

	UPROPERTY(Config,EditAnywhere, Category = Display)
	float zOrder = 10000;

	UPROPERTY(Transient, EditAnywhere, Category=Debugging)
	bool HoldLoadingScreenAdditionalSecsEvenInEditor = false;
};
