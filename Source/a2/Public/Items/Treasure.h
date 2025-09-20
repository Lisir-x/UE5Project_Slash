// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class A2_API ATreasure : public AItem
{
	GENERATED_BODY()
	
public:
	//ATreasure();

protected:
	//�ص��¼�
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;

private:
	//�ƽ�����
	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;

public:
	//��ȡ�ƽ�����
	FORCEINLINE int32 GetGold() const { return Gold; }
};
