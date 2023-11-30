// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASGrenade.h"
#include <Kismet/GameplayStatics.h>

AASGrenade::AASGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AASGrenade::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AASGrenade::Explode, countDownTime);
}

void AASGrenade::Explode()
{
    UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());

    if (!Explosion)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn particle system component."));
        return;
    }

    UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
    if (!ExplosionSound)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn sound system component."));
        return;
    }

    TArray<FHitResult> HitActors;

}

