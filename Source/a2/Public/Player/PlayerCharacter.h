
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

	//每一帧都调用
	virtual void Tick(float DeltaTime) override;

	//用于绑定输入
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//跳跃函数
	virtual void Jump() override;

	//受伤函数
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;

	//命中函数，受到伤害时调用
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	//设置当前重叠的物品
	virtual void SetOverlappingItem(AItem* Item) override;

	//添加灵魂
	virtual void AddSouls(ASoul* Soul) override;

	//添加黄金
	virtual void AddGold(ATreasure* Treasure) override;

protected:
	//游戏开始或生成时调用
	virtual void BeginPlay() override;

	/*----------输入处理函数----------*/
	//移动输入处理
	void Move(const FInputActionValue& Value);
	//视角输入处理
	void Look(const FInputActionValue& Value);
	////跳跃
	//void Jump();
	//F键输入处理
	void FKeyPressed();
	//攻击
	virtual void Attack() override;
	//闪避
	void Dodge();
	/*--------------------------------*/

	/*----------战斗相关----------*/
	//装备武器
	void EquipWeapon(ABaseWeapon* Weapon);
	//攻击结束
	virtual void AttackEnd() override;
	//闪避结束
	virtual void DodgeEnd() override;
	//判断是否可以攻击
	virtual bool CanAttack() override;
	//判断是否可以卸装
	bool CanDisarm();
	//判断是否可以装备
	bool CanArm();
	//判断是否有足够体力
	bool HasEnoughStamina();
	//判断当前是否空闲状态
	bool IsUnoccupied();
	//卸装
	void Disarm();
	//装备
	void Arm();
	//播放装备蒙太奇
	void PlayEquipMontage(const FName& SectionName);
	//死亡逻辑函数
	virtual void Die_Implementation() override;

	//卸装(附在背上)
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	//装备(附在手上)
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	//完成装备
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	//受击结束
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
	/*----------------------------*/

private:
	/*----------UI相关----------*/
	//初始化玩家UI
	void InitializePlayerOverlay();
	//设置血量到UI
	void SetHUDHealth();
	/*--------------------------*/

	/*----------组件----------*/
	//弹簧臂组件
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	//摄像机组件
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> Camera;
	/*------------------------*/

	/*----------输入相关----------*/
	//输入映射
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Player;
	//移动
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;
	//视角
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;
	//跳跃
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;
	//拾取或装备
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Equip;
	//攻击
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Attack;
	//闪避
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Dodge;
	/*----------------------------*/

	//武器或物品
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;

	/*----------动画蒙太奇----------*/
	//装备蒙太奇
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> EquipMontage;
	/*------------------------------*/
	
	//角色状态
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	//动作状态
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	//
	UPROPERTY()
	TObjectPtr<UPlayerOverlay> PlayerOverlay;

public:
	//获取角色状态
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	//获取动作状态
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};

