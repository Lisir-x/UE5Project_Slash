// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerOverlay;

UCLASS()
class A2_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	//游戏开始或生成时调用
	virtual void BeginPlay() override;

private:
	//血条类
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	//血条对象
	UPROPERTY()
	TObjectPtr<UPlayerOverlay> PlayerOverlay;

public:
	//获取血条对象
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() const { return PlayerOverlay; }
};
