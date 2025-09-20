// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

//设置血条百分比
void UPlayerOverlay::SetHealthBarPercent(float Percent)
{

	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

//设置体力条百分比
void UPlayerOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

//设置金币数量
void UPlayerOverlay::SetGold(int32 Gold)
{
	if (GoldText)
	{
		const FString String = FString::Printf(TEXT("%d"), Gold);
		const FText Text = FText::FromString(String); //FString转换为FText
		GoldText->SetText(Text);	//设置文本
	}
}

//设置灵魂数量
void UPlayerOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		const FString String = FString::Printf(TEXT("%d"), Souls);
		const FText Text = FText::FromString(String); //FString转换为FText
		SoulsText->SetText(Text);	//设置文本
	}
}
