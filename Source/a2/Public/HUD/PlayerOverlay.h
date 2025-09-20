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
	//����Ѫ���ٷֱ�
	void SetHealthBarPercent(float Percent);
	//�����������ٷֱ�
	void SetStaminaBarPercent(float Percent);
	//���ý������
	void SetGold(int32 Gold);
	//�����������
	void SetSouls(int32 Souls);

private:
	//����ͼ�ؼ���HealthBar(Ѫ��)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	//����ͼ�ؼ���StaminaBar(������)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;

	//����ͼ�ؼ���GoldText(���)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldText;

	//����ͼ�ؼ���SoulsText(���)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SoulsText;
};
