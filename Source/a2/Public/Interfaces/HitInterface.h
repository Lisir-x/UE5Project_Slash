// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

//��UInterface�����������޸ģ������Զ�����
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class A2_API IHitInterface
{
	GENERATED_BODY()

	//���ӿں��������ڴ˴����̳иýӿڵ�����Ҫ�ڴ�ʵ�־����߼�
public:
	//���к���
	UFUNCTION(BlueprintNativeEvent)//��������ͼ����д
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
};
