// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/ASItemPrimaryData.h"
#include "ASItemBase.generated.h"

//struct FItemBaseData;
enum class E_ItemState : uint8
{
	E_None,
	E_Drop,
	E_PickUp,
	E_Equip,
};

UCLASS()
class AOS_API AASItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AASItemBase();
public:

	void SetItemProperties(E_ItemState state);
	
	virtual void RefreshItem();
	virtual void Tick(float DeltaTime) override;

	virtual void CreateItem(int32 _itemID);
	virtual void CopyItem(AASItemBase* item);


	FORCEINLINE class USphereComponent* GetAreaSphere() const { return areaSphere; }
	FORCEINLINE class UBoxComponent* GetCollisionBox() const { return collisionBox; }
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return itemMeshComponent; }
	FORCEINLINE int GetItemID() { return itemID; }
protected:
	virtual void BeginPlay() override;
	virtual void SetTexture();
	virtual void SetMesh();
	virtual void SetCount();
	virtual	void UpdateItem();
	virtual void OnConstruction(const FTransform& Transform) override;

	void BounceItem();
	void StopBounceItem();
protected:
	UPROPERTY(EditAnywhere, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 itemID;

	UPROPERTY(EditAnywhere,Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 itemCount;

	UPROPERTY(VisibleAnyWhere,BlueprintReadOnly, Category = Item)
	class USkeletalMeshComponent* itemMeshComponent;

	UPROPERTY(VisibleAnyWhere)
	class UBoxComponent* collisionBox;

	UPROPERTY(VisibleAnyWhere)
	class UStaticMeshComponent* staticMesh;

	class UASItemPrimaryData* itemDataTable;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FItemBaseData itemBaseData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconItemTexture;

	E_ItemState currentState;

	UPROPERTY(VisibleAnyWhere)
	class USceneComponent* root;

	UPROPERTY(VisibleAnyWhere)
	class USceneComponent* particle;

	UPROPERTY(VisibleAnyWhere)
	class UParticleSystem* lootParticle;
private:

	UPROPERTY(VisibleAnyWhere,Category = Item)
	class USphereComponent* areaSphere;

	FTimerHandle BounceItemTimer;

	UPROPERTY(EditAnywhere, Category = Item)
	float BounceItemTime;

	bool bFalling;
};
