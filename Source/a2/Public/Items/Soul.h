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
	//ÿһ֡������
	virtual void Tick(float DeltaTime) override;

protected:
	//��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;
	//�ص��¼�
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult) override;

private:
	//�������
	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls;

	//Ŀ��߶�
	double DesiredZ;

	//
	UPROPERTY(EditAnywhere)
	float DriftRate = -15.f;

public:
	//��ȡ�������
	FORCEINLINE int32 GetSouls() const { return Souls; }

	//�����������
	FORCEINLINE void SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }
};
