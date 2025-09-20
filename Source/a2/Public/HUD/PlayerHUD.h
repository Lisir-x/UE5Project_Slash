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
	//��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;

private:
	//Ѫ����
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	//Ѫ������
	UPROPERTY()
	TObjectPtr<UPlayerOverlay> PlayerOverlay;

public:
	//��ȡѪ������
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() const { return PlayerOverlay; }
};
