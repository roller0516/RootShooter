#include "Item/ASAmmo.h"
#include "Character/ASPlayerCharacter.h"

AASAmmo::AASAmmo()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AASAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

void AASAmmo::UseItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	auto playerCharacter = Cast<AASPlayerCharacter>(OtherActor);
	if (playerCharacter)
	{
		playerCharacter->PickupAmmoItem(30);
		Destroy();
	}
}