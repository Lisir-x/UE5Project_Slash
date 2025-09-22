// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "CharacterType.h"
#include "BaseCharacter.generated.h"

class ABaseWeapon;
class UAnimMontage;
class UAttributeComponent;

UCLASS()
class A2_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	//每一帧都调用
	virtual void Tick(float DeltaTime) override;

protected:
	//游戏开始或生成时调用
	virtual void BeginPlay() override;

	/*----------战斗相关----------*/
	//命中函数，受到伤害时调用
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	//攻击
	virtual void Attack();

	//死亡逻辑
	UFUNCTION(BlueprintNativeEvent)
	void Die();

	//方向性受击反应
	void DirectionalHitReact(const FVector& ImpactPoint);
	//处理伤害
	virtual void HandleDamage(float DamageAmount);
	//播放受击音效
	void PlayHitSound(const FVector& ImpactPoint);
	//播放受击粒子效果
	void SpawnHitParticles(const FVector& ImpactPoint);
	//禁用胶囊体碰撞
	void DisableCapsule();
	//判断是否可以攻击
	virtual bool CanAttack();
	//判断是否存活
	bool IsAlive();
	//禁用网格体碰撞
	void DisableMeshCollision();
	/*----------------------------*/

	/*----------蒙太奇相关----------*/
	//播放受击蒙太奇
	void PlayHitReactMontage(const FName& SectionName);
	//播放攻击蒙太奇
	virtual int32 PlayAttackMontage();
	//播放死亡蒙太奇
	virtual int32 PlayDeathMontage();
	//中断攻击蒙太奇
	void StopAttackMontage();
	//播放闪避蒙太奇
	virtual void PlayDodgeMontage();
	/*------------------------------*/

	//获取扭曲目标位移
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	//获取扭曲目标旋转
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	//攻击结束
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	//闪避结束
	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	//设置武器碰撞类型
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable);

	//设置骨骼网格体碰撞属性
	UFUNCTION(BlueprintCallable)
	void SetMeshCollisionResponses(
		const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Responses);

	//已装备的武器
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<ABaseWeapon> EquippedWeapon;

	//属性组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;

	//当前战斗目标
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	//扭曲目标范围
	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;

	//死亡姿势
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

private:

	//播放指定蒙太奇的指定段
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	//随机播放蒙太奇段
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	//受击音效
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USoundBase> HitSound;

	//粒子系统
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UParticleSystem> HitParticles;

	/*----------动画蒙太奇----------*/
	//攻击蒙太奇
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

	//受击蒙太奇
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	//死亡蒙太奇
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> DeathMontage;

	//闪避蒙太奇
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> DodgeMontage;

	//攻击蒙太奇段名称
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackMontageSections;

	//死亡蒙太奇段名称
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> DeathMontageSections;
	/*------------------------------*/

public:
	//获取死亡姿势
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
};

