
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "BaseCharacter.h"
#include "CharacterType.h"
#include "Interfaces/PickupInterface.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class ASoul;
class ATreasure;
class UAnimMontage;
class UPlayerOverlay;

UCLASS()
class A2_API APlayerCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	//ÿһ֡������
	virtual void Tick(float DeltaTime) override;

	//���ڰ�����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//��Ծ����
	virtual void Jump() override;

	//���˺���
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;

	//���к������ܵ��˺�ʱ����
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	//���õ�ǰ�ص�����Ʒ
	virtual void SetOverlappingItem(AItem* Item) override;

	//������
	virtual void AddSouls(ASoul* Soul) override;

	//��ӻƽ�
	virtual void AddGold(ATreasure* Treasure) override;

protected:
	//��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;

	/*----------���봦����----------*/
	//�ƶ����봦��
	void Move(const FInputActionValue& Value);
	//�ӽ����봦��
	void Look(const FInputActionValue& Value);
	////��Ծ
	//void Jump();
	//F�����봦��
	void FKeyPressed();
	//����
	virtual void Attack() override;
	//����
	void Dodge();
	/*--------------------------------*/

	/*----------ս�����----------*/
	//װ������
	void EquipWeapon(ABaseWeapon* Weapon);
	//��������
	virtual void AttackEnd() override;
	//���ܽ���
	virtual void DodgeEnd() override;
	//�ж��Ƿ���Թ���
	virtual bool CanAttack() override;
	//�ж��Ƿ����жװ
	bool CanDisarm();
	//�ж��Ƿ����װ��
	bool CanArm();
	//�ж��Ƿ����㹻����
	bool HasEnoughStamina();
	//�жϵ�ǰ�Ƿ����״̬
	bool IsUnoccupied();
	//жװ
	void Disarm();
	//װ��
	void Arm();
	//����װ����̫��
	void PlayEquipMontage(const FName& SectionName);
	//�����߼�����
	virtual void Die_Implementation() override;

	//жװ(���ڱ���)
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	//װ��(��������)
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	//���װ��
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	//�ܻ�����
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
	/*----------------------------*/

private:
	/*----------UI���----------*/
	//��ʼ�����UI
	void InitializePlayerOverlay();
	//����Ѫ����UI
	void SetHUDHealth();
	/*--------------------------*/

	/*----------���----------*/
	//���ɱ����
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	//��������
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> Camera;
	/*------------------------*/

	/*----------�������----------*/
	//����ӳ��
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Player;
	//�ƶ�
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;
	//�ӽ�
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;
	//��Ծ
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;
	//ʰȡ��װ��
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Equip;
	//����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Attack;
	//����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Dodge;
	/*----------------------------*/

	//��������Ʒ
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;

	/*----------������̫��----------*/
	//װ����̫��
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> EquipMontage;
	/*------------------------------*/
	
	//��ɫ״̬
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	//����״̬
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	//
	UPROPERTY()
	TObjectPtr<UPlayerOverlay> PlayerOverlay;

public:
	//��ȡ��ɫ״̬
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	//��ȡ����״̬
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
