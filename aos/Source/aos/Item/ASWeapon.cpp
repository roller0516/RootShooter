// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Data/ASItemPrimaryData.h"
#include "Animation/AnimInstance.h"
#include "ASItemBase.h"
#include "Engine/Texture2D.h"

//#include "Item/ASItemBase.h"
AASWeapon::AASWeapon()
{
	
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

FTransform AASWeapon::GetRightHandSocket()
{
	const FTransform RightHandTr = itemMeshComponent->GetSocketTransform("RightHand");

	return RightHandTr;
}

FName AASWeapon::GetReloadMotageSection()
{
	if(weaponType == WeaponType::SMG)
	{
		return FName("StartReloading");
	}
	else if(weaponType == WeaponType::Rifle)
	{
		return FName("ReloadAR");
	}

	return FName();
}

void AASWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AASWeapon::UpdateItem()
{
	SetMesh();

	SetCount();

	SetMaxAmmoCount();

	SetDamage();

	SetTexture();

	SetMagazine();

	ResetAmmo();

	SetAnimInstance();

	SetCrossHair();

	ClipBoneName = weaponData.ClipBoneName;

	ShellEffect = weaponData.ShellEffect;
	MuzzleEffect = weaponData.MuzzleEffect;
	TracerEffect = weaponData.TracerEffect;
	ShellMesh = weaponData.ShellMesh;
}

void AASWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AASWeapon::OnConstruction(const FTransform& Transform)
{	
	Super::OnConstruction(Transform);
}

void AASWeapon::SetTexture()
{
	Super::SetTexture();

	AmmoItemTexture = weaponData.ammoTexture;
}

void AASWeapon::SetMesh()
{ 
	itemMeshComponent->SetSkeletalMesh(weaponData.mesh);
}

void AASWeapon::SetCount()
{
	itemCount = weaponData.Count;
}

void AASWeapon::SetDamage()
{
	MinDamage = weaponData.MinDamage;
	MaxDamage = weaponData.MaxDamage;
}

void AASWeapon::ResetAmmo()
{
	if(magazineCapacity <= 0) return;

	int chai = maxAmmoCount - curAmmonCount;

	if (chai > magazineCapacity)
	{
		curAmmonCount = magazineCapacity;
		magazineCapacity = 0;
	}
	else
	{
		curAmmonCount = maxAmmoCount;
		magazineCapacity -= chai;
	}
}

void AASWeapon::SetAnimInstance()
{
	itemMeshComponent->SetAnimInstanceClass(weaponData.animInstance);
}

void AASWeapon::SetMaxAmmoCount()
{
	maxAmmoCount = weaponData.maxAmmoCount;
}

void AASWeapon::SetMagazine()
{
	magazineCapacity = weaponData.defaultMagazineCapacity;
}

void AASWeapon::SetCrossHair()
{
	if(weaponData.CrosshairsMiddle)
	{
		CrosshairsMiddle = weaponData.CrosshairsMiddle;
	}

	if(weaponData.CrosshairsLeft)
	{
		CrosshairsLeft = weaponData.CrosshairsLeft;
	}

	if(weaponData.CrosshairsBottom)
	{
		CrosshairsBottom = weaponData.CrosshairsBottom;
	}

	if(weaponData.CrosshairsRight)
	{
		CrosshairsRight = weaponData.CrosshairsRight;
	}

	if(weaponData.CrosshairsTop)
	{
		CrosshairsTop = weaponData.CrosshairsTop;
	}
}

void AASWeapon::DecrementAmmo()
{
	curAmmonCount = FMath::Clamp(curAmmonCount -= 1, 0, maxAmmoCount);
}

void AASWeapon::CreateItem(int32 _itemID)
{
	Super::CreateItem(_itemID);

	if(itemDataTable->GetWeaponData(itemID))
	{
		weaponData = *itemDataTable->GetWeaponData(itemID);

		weaponType = weaponData.weaponType;

		UpdateItem();
	}
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
