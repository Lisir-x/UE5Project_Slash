// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class A2_API ASoul : public AItem
{
	GENERATED_BODY()
	
public:
	//每一帧都调用
	virtual void Tick(float DeltaTime) override;

protected:
	//游戏开始或生成时调用
	virtual void BeginPlay() override;
	//重叠事件
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult) override;

private:
	//灵魂数量
	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls;

	//目标高度
	double DesiredZ;

	//
	UPROPERTY(EditAnywhere)
	float DriftRate = -15.f;

public:
	//获取灵魂数量
	FORCEINLINE int32 GetSouls() const { return Souls; }

	//设置灵魂数量
	FORCEINLINE void SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }
};

