// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//尝试将重叠物体转换为拾取接口
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		//调用接口函数，添加黄金
		PickupInterface->AddGold(this);
		//播放拾取音效
		SpawnPickupSound();
		//销毁拾取物
		Destroy();
	}
}

