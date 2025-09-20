// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"

//��Ϸ��ʼ������ʱ����
void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	//��ȡ����
	UWorld* World = GetWorld();
	if (World)
	{
		//��ȡ��ҿ�����
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && PlayerOverlayClass)
		{
			//����Ѫ������
			PlayerOverlay = CreateWidget<UPlayerOverlay>(Controller, PlayerOverlayClass);
			//��ӵ��ӿ�
			PlayerOverlay->AddToViewport();
		}
	}
}
