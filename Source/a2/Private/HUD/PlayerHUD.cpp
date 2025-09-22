// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"

//游戏开始或生成时调用
void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	//获取世界
	UWorld* World = GetWorld();
	if (World)
	{
		//获取玩家控制器
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && PlayerOverlayClass)
		{
			//创建血条对象
			PlayerOverlay = CreateWidget<UPlayerOverlay>(Controller, PlayerOverlayClass);
			//添加到视口
			PlayerOverlay->AddToViewport();
		}
	}
}

