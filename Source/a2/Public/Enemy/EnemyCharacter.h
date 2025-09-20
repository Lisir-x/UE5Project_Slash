// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "Player/CharacterType.h"
#include "EnemyCharacter.generated.h"

class UHealthbarComponent;
class AAIController;
class UPawnSensingComponent;
//class UAIPerceptionComponent;
//class UAISenseConfig_Sight;
//class UAISenseConfig_Hearing;

UCLASS()
class A2_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	/** <AActor> */
	//每一帧都调用
	virtual void Tick(float DeltaTime) override;
	//受伤函数
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	//销毁时调用
	virtual void Destroyed() override;
	/** </AActor> */

	/** <IHitInterface> */
	//命中函数，受到伤害时调用
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

protected:
	/** <AActor> */
	//游戏开始或生成时调用
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	//死亡逻辑
	virtual void Die_Implementation() override;
	//生成灵魂
	void SpawnSoul();
	//敌人攻击
	virtual void Attack() override;
	//判断是否可以攻击
	virtual bool CanAttack() override;
	//攻击结束
	virtual void AttackEnd() override;
	//处理伤害
	virtual void HandleDamage(float DamageAmount) override;
	/** </ABaseCharacter> */

	//敌人当前状态
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
	/*----------AI行为相关----------*/
	//初始化敌人
	void InitializeEnemy();
	//选择巡逻目标
	void CheckPatrolTarget();
	//选择战斗目标
	void CheckCombatTarget();
	//巡逻等待时间回调函数
	void PatrolTimerFinished();
	//隐藏血条
	void HideHealthBar();
	//显示血条
	void ShowHealthBar();
	//失去兴趣
	void LoseInterest();
	//开始巡逻
	void StartPatrolling();
	//追逐目标
	void ChaseTarget();
	//是否超出战斗范围
	bool IsOutsideCombatRadius();
	//是否超出攻击范围
	bool IsOutsideAttackRadius();
	//是否在攻击范围内
	bool IsInsideAttackRadius();
	//是否处于巡逻状态
	bool IsChasing();
	//是否处于攻击状态
	bool IsAttacking();
	//是否处于死亡状态
	bool IsDead();
	//是否处于忙碌状态
	bool IsEngaged();
	//清除巡逻计时器
	void ClearPatrolTimer();
	//启用攻击计时器
	void StartAttackTimer();
	//清除攻击计时器
	void ClearAttackTimer();
	//检查目标是否在战斗范围内
	bool InTargetRange(AActor* Target, double Radius);
	//移动到目标
	void MoveToTarget(AActor* Target);
	//选择新的巡逻点
	AActor* ChoosePatrolTarget();
	//生成默认武器
	void SpawnDefaultWeapon();
	/*------------------------------*/

	/*----------回调函数----------*/
	//Pawn感知回调函数
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	////AI感知回调函数
	//UFUNCTION()
	//void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	/*----------------------------*/

	/*----------组件----------*/
	//血条组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthbarComponent> HealthBarWidget;

	//Pawn感知组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensing;

	////AI感知组件
	//UPROPERTY(VisibleAnywhere, Category = "AI")
	//TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	//UPROPERTY(VisibleAnywhere, Category = "AI")
	//TObjectPtr<UAISenseConfig_Sight> SightConfig;

	//UPROPERTY(VisibleAnywhere, Category = "AI")
	//TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	/*------------------------*/

	/*----------导航相关----------*/
	//AI控制器
	UPROPERTY()
	TObjectPtr<AAIController> EnemyController;

	//当前巡逻点
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	//巡逻点索引
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets;

	//巡逻半径
	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrolRadius = 200.f;

	//巡逻等待时间
	FTimerHandle PatrolTimer;

	//最小和最大等待时间
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	//巡逻速度
	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrollingSpeed = 125.f;
	/*----------------------------*/

	/*----------战斗相关----------*/
	//敌人武器
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<ABaseWeapon> WeaponClass;

	//战斗半径
	UPROPERTY(EditAnywhere, Category = "Combat")
	float CombatRadius = 1500.f;

	//攻击范围
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRadius = 200.f;

	//接受半径	
	UPROPERTY(EditAnywhere, Category = "Combat")
	double AcceptanceRadius = 100.f;

	//攻击计时器
	FTimerHandle AttackTimer;

	//最小和最大攻击间隔时间
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;

	//追逐速度
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 300.f;

	//死亡延迟销毁时间
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.f;

	//灵魂类
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class ASoul> SoulClass;
	/*----------------------------*/
};
