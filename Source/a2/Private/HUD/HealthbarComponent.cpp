// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthbarComponent.h"
#include "HUD/Healthbar.h"
#include "Components/ProgressBar.h"

void UHealthbarComponent::SetHealthPercent(float Percent)
{
	//��UUserWidget��ָ��ת����UHealthBar��ָ��
	if (!HealthBarWidget)	//ֻ����һ��ת��
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		//����Ѫ���ٷֱ�
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
