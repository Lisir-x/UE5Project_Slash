// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���Խ��ص�����ת��Ϊʰȡ�ӿ�
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		//���ýӿں�������ӻƽ�
		PickupInterface->AddGold(this);
		//����ʰȡ��Ч
		SpawnPickupSound();
		//����ʰȡ��
		Destroy();
	}
}
