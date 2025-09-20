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
	//���������ٷֱ�
	void SetHealthPercent(float Percent);

	//Ѫ��
	UPROPERTY()
	TObjectPtr<UHealthBar> HealthBarWidget;

};
