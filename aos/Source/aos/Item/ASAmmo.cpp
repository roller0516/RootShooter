#include "Item/ASAmmo.h"
#include "Components/BoxComponent.h"
#include "Character/ASPlayerCharacter.h"

AASAmmo::AASAmmo()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox")); 
	OverlapBox->SetupAttachment(RootComponent);
	OverlapBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AASAmmo::UseItem);
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
		playerCharacter->GetAmmo(30);
		Destroy();
	}
}