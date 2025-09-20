// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class A2_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//设置血条百分比
	void SetHealthBarPercent(float Percent);
	//设置体力条百分比
	void SetStaminaBarPercent(float Percent);
	//设置金币数量
	void SetGold(int32 Gold);
	//设置灵魂数量
	void SetSouls(int32 Souls);

private:
	//绑定蓝图控件的HealthBar(血条)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	//绑定蓝图控件的StaminaBar(体力条)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;

	//绑定蓝图控件的GoldText(金币)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldText;

	//绑定蓝图控件的SoulsText(灵魂)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SoulsText;
};
