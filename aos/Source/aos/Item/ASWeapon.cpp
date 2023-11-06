// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "NiagaraComponent.h"

//#include "Item/ASItemBase.h"
AASWeapon::AASWeapon()
{
	ClipBoneName = TEXT("smg_clip");
}

void AASWeapon::ShowShotParticles()
{
	const USkeletalMeshSocket* BarrelSocket = ItemMesh->GetSocketByName("BarrelSocket");
	
	if (BarrelSocket) 
	{
		const FTransform muzzleTr = BarrelSocket->GetSocketTransform(ItemMesh);
		
		
		FVector v = muzzleTr.GetLocation();
		if (MuzzleEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleEffect, muzzleTr.GetLocation());
		}

		if (ShellEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShellEffect, muzzleTr.GetLocation());
			
		}

		if (TracerEffect)
		{
			UNiagaraComponent* trace = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TracerEffect, muzzleTr.GetLocation());
			trace->SetVectorParameter(FName("Target"), muzzleTr.GetLocation());
		}
	}
}

FTransform AASWeapon::GetBarrelSocketTransForm() const
{
	const USkeletalMeshSocket* BarrelSocket = ItemMesh->GetSocketByName("BarrelSocket");
	if(BarrelSocket)
		return BarrelSocket->GetSocketTransform(ItemMesh);
	return FTransform();
}
