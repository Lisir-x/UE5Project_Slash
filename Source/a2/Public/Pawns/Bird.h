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
	//ÿһ֡������
	virtual void Tick(float DeltaTime) override;

	//���ڰ�����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;

	//�ƶ����봦��
	void BirdMove(const FInputActionValue& Value);

	//�ӽ����봦��
	void BirdLook(const FInputActionValue& Value);

private:
	//�������
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> Capsule;

	//�������������
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BirdMesh;

	//���ɱ����
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	//��������
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	//�ƶ����
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMyFloatingPawnMovement> PawnMovement;

	//����ӳ��
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Bird;

	//�ƶ�
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_BirdMove;

	//�ӽ�
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_BirdLook;
};
