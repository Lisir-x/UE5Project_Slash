// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

//此UInterface派生类无需修改（引擎自动处理）
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

	//将接口函数声明在此处，继承该接口的类需要在此实现具体逻辑
public:
	//命中函数
	UFUNCTION(BlueprintNativeEvent)//允许在蓝图中重写
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
};

