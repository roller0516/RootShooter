// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Data/ASItemPrimaryData.h"
#include "Data/ASItemData.h"
#include "ASItemBase.h"

//#include "Item/ASItemBase.h"
AASWeapon::AASWeapon()
{
	ClipBoneName = TEXT("smg_clip");
}

void AASWeapon::ShowShotParticles(FHitResult pHitResult)
{
	const USkeletalMeshSocket* BarrelSocket = itemMeshComponent->GetSocketByName("BarrelSocket");
	const USkeletalMeshSocket* ShellSocket = itemMeshComponent->GetSocketByName("BarrelSocket");

	if(ShellSocket)
	{
		const FTransform ShellSocketTr = BarrelSocket->GetSocketTransform(itemMeshComponent);
		FVector v = ShellSocketTr.GetLocation();
		FRotator r = ShellSocketTr.GetRotation().Rotator();
		if (ShellEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShellEffect, ShellSocketTr.GetLocation());
			UNiagaraComponent* tempTracer =
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					ShellEffect, itemMeshComponent, TEXT("ShellSocket"),
					ShellSocketTr.GetLocation(), r,
					EAttachLocation::KeepWorldPosition, true);
			tempTracer->SetNiagaraVariableObject(FString(TEXT("User.ShellEjectStaticMesh")), ShellMesh);
			tempTracer->SetNiagaraVariableBool(FString(TEXT("User.Trigger")), true);
		}
	}

	if (BarrelSocket) 
	{
		const FTransform muzzleTr = BarrelSocket->GetSocketTransform(itemMeshComponent);
		
		FVector v = muzzleTr.GetLocation();
		FRotator r = muzzleTr.GetRotation().Rotator();

		FVector hitPosition = pHitResult.bBlockingHit ? pHitResult.ImpactPoint : pHitResult.TraceEnd;

		if (MuzzleEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleEffect, muzzleTr.GetLocation());
			UNiagaraComponent* muzzle =
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					MuzzleEffect, itemMeshComponent, TEXT("BarrelSocket"),
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
					TracerEffect, itemMeshComponent, TEXT("BarrelSocket"),
					muzzleTr.GetLocation(), r,
					EAttachLocation::KeepWorldPosition, true);
			tempTracer->SetNiagaraVariableBool(FString("User.Trigger"), true);

			TArray<FVector> TraceImpactPosArr;
			TraceImpactPosArr.Add(pHitResult.bBlockingHit ? pHitResult.ImpactPoint : pHitResult.TraceEnd);

			UNiagaraDataInterfaceArrayFunctionLibrary::
				SetNiagaraArrayVector(tempTracer, TEXT("User.ImpactPositions"), TraceImpactPosArr);

			tempTracer->SetNiagaraVariablePosition(
				FString(TEXT("User.MuzzlePostion")), itemMeshComponent->GetSocketLocation(TEXT("BarrelSocket")));
		}
	}
}

FTransform AASWeapon::GetBarrelSocketTransForm() const
{
	const USkeletalMeshSocket* BarrelSocket = itemMeshComponent->GetSocketByName("BarrelSocket");
	if(BarrelSocket)
		return BarrelSocket->GetSocketTransform(itemMeshComponent);
	return FTransform();
}

void AASWeapon::CreateWeapon(int32 _itemID)
{
	itemID = _itemID;
	weaponData = (FWeaponData*)itemDataTable->GetItemData(itemID);

	SetMesh();

	SetCount();

	SetMaxAmmoCount();

	curAmmonCount = maxAmmoCount;

	SetActorScale3D(FVector(1,1,1));
}

void AASWeapon::BeginPlay()
{
	Super::BeginPlay();

	weaponData = (FWeaponData*)itemDataTable->GetItemData(itemID);
}

void AASWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AASWeapon::SetTexture()
{
	
}

void AASWeapon::SetMesh()
{
	if(weaponData)
		itemMeshComponent->SetSkeletalMesh(weaponData->mesh.Get());
}

void AASWeapon::SetCount()
{
	if(itemID != 0)
		itemCount = itemDataTable->GetItemData(itemID)->Count;
}

void AASWeapon::ResetCurAmmoCount(int32 count)
{
	curAmmonCount = maxAmmoCount;
}

void AASWeapon::SetMaxAmmoCount()
{
	if(weaponData)
		maxAmmoCount = weaponData->maxAmmoCount;
}

void AASWeapon::DecrementAmmo()
{
	curAmmonCount = FMath::Clamp(curAmmonCount -= 1, 0, maxAmmoCount);
}

float AASWeapon::GetDamage()
{
	FRandomStream RandomStream(FDateTime::Now().GetTicks());

	int32 RandomValue = RandomStream.RandRange(MinDamage, MaxDamage);

	return RandomValue;
}

float AASWeapon::GetHeadDamage()
{
	return GetDamage() * 2;

}
