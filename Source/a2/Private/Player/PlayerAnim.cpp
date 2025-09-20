

#include "Player/PlayerAnim.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

//��ʼ������ʵ��
void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//��ȡ��ɫ���ƶ����
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		CharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
	
}

//���¶���ʵ��
void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//ȷ����ɫ���ƶ������Ч
	if (PlayerCharacter && CharacterMovement)
	{
		//�����ٶ�
		Speed = CharacterMovement->Velocity.Size();
		//�����Ƿ��ڿ���
		IsFalling = CharacterMovement->IsFalling();
		//���½�ɫ״̬
		CharacterState = PlayerCharacter->GetCharacterState();
		//���¶���״̬
		ActionState = PlayerCharacter->GetActionState();
		//������������
		DeathPose = PlayerCharacter->GetDeathPose();
	}

}
