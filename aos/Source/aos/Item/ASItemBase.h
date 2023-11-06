// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASItemBase.generated.h"

UCLASS()
class AOS_API AASItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AASItemBase();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere,Category = Item)
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere,Category = Item)
	class USphereComponent* AreaSphere;
protected:
	UPROPERTY(EditAnywhere, Category = Item)
	class USkeletalMeshComponent* ItemMesh;
public:
	FORCEINLINE class USphereComponent* GetAreaSphere() const {return AreaSphere;}
	FORCEINLINE class UBoxComponent* GetCollisionBox() const {return CollisionBox;}
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const {return ItemMesh;}
};
