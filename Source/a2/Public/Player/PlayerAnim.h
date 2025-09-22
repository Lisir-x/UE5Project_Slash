
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
	//初始化动画实例
	virtual void NativeInitializeAnimation() override;
	//更新动画实例
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//角色
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	//角色移动组件
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	//速度
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;

	//是否在空中
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsFalling;

	//角色状态
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	ECharacterState CharacterState;

	//动作状态
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EActionState ActionState;

	//死亡姿势
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TEnumAsByte<EDeathPose> DeathPose;

};

