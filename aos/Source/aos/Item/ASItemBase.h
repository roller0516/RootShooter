// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/ASItemPrimaryData.h"
#include "ASItemBase.generated.h"

//struct FItemBaseData;

UCLASS()
class AOS_API AASItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AASItemBase();
public:
	FORCEINLINE class USphereComponent* GetAreaSphere() const { return areaSphere; }
	FORCEINLINE class UBoxComponent* GetCollisionBox() const { return collisionBox; }
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return itemMeshComponent; }

	virtual void CreateItem(int32 _itemID);
protected:
	virtual void BeginPlay() override;
	virtual void SetTexture();
	virtual void SetMesh();
	virtual void SetCount();
	virtual	void UpdateItem();
protected:
	UPROPERTY(VisibleAnywhere, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 itemID;

	UPROPERTY(VisibleAnywhere,Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 itemCount;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Item)
	class USkeletalMeshComponent* itemMeshComponent;

	class UASItemPrimaryData* itemDataTable;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FItemBaseData itemBaseData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconItemTexture;
private:
	UPROPERTY(EditAnywhere,Category = Item)
	class UBoxComponent* collisionBox;

	UPROPERTY(EditAnywhere,Category = Item)
	class USphereComponent* areaSphere;
};
