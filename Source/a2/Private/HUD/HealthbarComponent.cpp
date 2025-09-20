// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthbarComponent.h"
#include "HUD/Healthbar.h"
#include "Components/ProgressBar.h"

void UHealthbarComponent::SetHealthPercent(float Percent)
{
	//将UUserWidget类指针转换成UHealthBar类指针
	if (!HealthBarWidget)	//只进行一次转换
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		//设置血量百分比
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
