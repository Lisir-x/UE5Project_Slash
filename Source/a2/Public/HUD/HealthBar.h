// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;

UCLASS()
class A2_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//绑定蓝图控件的HealthBar(进度条属性)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

};

