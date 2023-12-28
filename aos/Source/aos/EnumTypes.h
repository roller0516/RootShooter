#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class CharID : uint8
{
	None,
	Belica,
	Blast
};

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	None,
	Rifle,
	SMG,
};


UENUM(BlueprintType)
enum class ItemGrade : uint8
{
	Normal,
	Rare,
	Epic,
	SpeicalRare,
	SuperSpeicalRare,
};
