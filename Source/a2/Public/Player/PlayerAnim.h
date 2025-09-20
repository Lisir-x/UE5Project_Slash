
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/CharacterType.h"
#include "PlayerAnim.generated.h"

class APlayerCharacter;
class UCharacterMovementComponent;

UCLASS()
class A2_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	//��ʼ������ʵ��
	virtual void NativeInitializeAnimation() override;
	//���¶���ʵ��
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//��ɫ
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	//��ɫ�ƶ����
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	//�ٶ�
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;

	//�Ƿ��ڿ���
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsFalling;

	//��ɫ״̬
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	ECharacterState CharacterState;

	//����״̬
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EActionState ActionState;

	//��������
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TEnumAsByte<EDeathPose> DeathPose;

};
