// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

//����Ѫ���ٷֱ�
void UPlayerOverlay::SetHealthBarPercent(float Percent)
{

	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

//�����������ٷֱ�
void UPlayerOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

//���ý������
void UPlayerOverlay::SetGold(int32 Gold)
{
	if (GoldText)
	{
		const FString String = FString::Printf(TEXT("%d"), Gold);
		const FText Text = FText::FromString(String); //FStringת��ΪFText
		GoldText->SetText(Text);	//�����ı�
	}
}

//�����������
void UPlayerOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		const FString String = FString::Printf(TEXT("%d"), Souls);
		const FText Text = FText::FromString(String); //FStringת��ΪFText
		SoulsText->SetText(Text);	//�����ı�
	}
}
