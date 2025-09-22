

#include "Player/PlayerAnim.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

//初始化动画实例
void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//获取角色和移动组件
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		CharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
	
}

//更新动画实例
void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//确保角色和移动组件有效
	if (PlayerCharacter && CharacterMovement)
	{
		//更新速度
		Speed = CharacterMovement->Velocity.Size();
		//更新是否在空中
		IsFalling = CharacterMovement->IsFalling();
		//更新角色状态
		CharacterState = PlayerCharacter->GetCharacterState();
		//更新动作状态
		ActionState = PlayerCharacter->GetActionState();
		//更新死亡姿势
		DeathPose = PlayerCharacter->GetDeathPose();
	}

}

