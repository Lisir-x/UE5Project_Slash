// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Bird.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UMyFloatingPawnMovement;
class UInputMappingContext;
class UInputAction;

UCLASS()
class A2_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();
	//每一帧都调用
	virtual void Tick(float DeltaTime) override;

	//用于绑定输入
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//游戏开始或生成时调用
	virtual void BeginPlay() override;

	//移动输入处理
	void BirdMove(const FInputActionValue& Value);

	//视角输入处理
	void BirdLook(const FInputActionValue& Value);

private:
	//胶囊组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> Capsule;

	//骨骼网格体组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BirdMesh;

	//弹簧臂组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	//摄像机组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	//移动组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMyFloatingPawnMovement> PawnMovement;

	//输入映射
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Bird;

	//移动
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_BirdMove;

	//视角
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_BirdLook;
};

