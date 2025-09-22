// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthbarComponent.generated.h"

class UHealthBar;

UCLASS()
class A2_API UHealthbarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	//设置生命百分比
	void SetHealthPercent(float Percent);

	//血条
	UPROPERTY()
	TObjectPtr<UHealthBar> HealthBarWidget;

};

