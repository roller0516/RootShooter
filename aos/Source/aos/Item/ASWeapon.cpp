// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

//#include "Item/ASItemBase.h"
AASWeapon::AASWeapon()
{
	ClipBoneName = TEXT("smg_clip");
}

void AASWeapon::ShowShotParticles(FHitResult pHitResult)
{
	const USkeletalMeshSocket* BarrelSocket = ItemMesh->GetSocketByName("BarrelSocket");
	const USkeletalMeshSocket* ShellSocket = ItemMesh->GetSocketByName("BarrelSocket");

	if(ShellSocket)
	{
		const FTransform ShellSocketTr = BarrelSocket->GetSocketTransform(ItemMesh);
		FVector v = ShellSocketTr.GetLocation();
		FRotator r = ShellSocketTr.GetRotation().Rotator();
		if (ShellEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShellEffect, ShellSocketTr.GetLocation());
			UNiagaraComponent* tempTracer =
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					ShellEffect, ItemMesh, TEXT("ShellSocket"),
					ShellSocketTr.GetLocation(), r,
					EAttachLocation::KeepWorldPosition, true);
			tempTracer->SetNiagaraVariableObject(FString(TEXT("User.ShellEjectStaticMesh")), ShellMesh);
			tempTracer->SetNiagaraVariableBool(FString(TEXT("User.Trigger")), true);
		}
	}

	if (BarrelSocket) 
	{
		const FTransform muzzleTr = BarrelSocket->GetSocketTransform(ItemMesh);
		
		FVector v = muzzleTr.GetLocation();
		FRotator r = muzzleTr.GetRotation().Rotator();

		FVector hitPosition = pHitResult.bBlockingHit ? pHitResult.ImpactPoint : pHitResult.TraceEnd;

		if (MuzzleEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleEffect, muzzleTr.GetLocation());
			UNiagaraComponent* muzzle =
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					MuzzleEffect, ItemMesh, TEXT("BarrelSocket"),
					muzzleTr.GetLocation(), r,
					EAttachLocation::KeepWorldPosition, true);
			FVector direction = hitPosition - v;
			direction.Normalize();

			muzzle->SetNiagaraVariableVec3(FString(TEXT("User.Direction")), direction);
			muzzle->SetNiagaraVariableBool(FString(TEXT("User.Trigger")), true);
		}

		if (TracerEffect)
		{
			UNiagaraComponent* tempTracer =
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					TracerEffect, ItemMesh, TEXT("BarrelSocket"),
					muzzleTr.GetLocation(), r,
					EAttachLocation::KeepWorldPosition, true);
			tempTracer->SetNiagaraVariableBool(FString("User.Trigger"), true);

			TArray<FVector> TraceImpactPosArr;
			TraceImpactPosArr.Add(pHitResult.bBlockingHit ? pHitResult.ImpactPoint : pHitResult.TraceEnd);

			UNiagaraDataInterfaceArrayFunctionLibrary::
				SetNiagaraArrayVector(tempTracer, TEXT("User.ImpactPositions"), TraceImpactPosArr);

			tempTracer->SetNiagaraVariablePosition(
				FString(TEXT("User.MuzzlePostion")), ItemMesh->GetSocketLocation(TEXT("BarrelSocket")));
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
