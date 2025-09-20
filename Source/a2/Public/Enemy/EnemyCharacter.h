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
	//ÿһ֡������
	virtual void Tick(float DeltaTime) override;
	//���˺���
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	//����ʱ����
	virtual void Destroyed() override;
	/** </AActor> */

	/** <IHitInterface> */
	//���к������ܵ��˺�ʱ����
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

protected:
	/** <AActor> */
	//��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	//�����߼�
	virtual void Die_Implementation() override;
	//�������
	void SpawnSoul();
	//���˹���
	virtual void Attack() override;
	//�ж��Ƿ���Թ���
	virtual bool CanAttack() override;
	//��������
	virtual void AttackEnd() override;
	//�����˺�
	virtual void HandleDamage(float DamageAmount) override;
	/** </ABaseCharacter> */

	//���˵�ǰ״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
	/*----------AI��Ϊ���----------*/
	//��ʼ������
	void InitializeEnemy();
	//ѡ��Ѳ��Ŀ��
	void CheckPatrolTarget();
	//ѡ��ս��Ŀ��
	void CheckCombatTarget();
	//Ѳ�ߵȴ�ʱ��ص�����
	void PatrolTimerFinished();
	//����Ѫ��
	void HideHealthBar();
	//��ʾѪ��
	void ShowHealthBar();
	//ʧȥ��Ȥ
	void LoseInterest();
	//��ʼѲ��
	void StartPatrolling();
	//׷��Ŀ��
	void ChaseTarget();
	//�Ƿ񳬳�ս����Χ
	bool IsOutsideCombatRadius();
	//�Ƿ񳬳�������Χ
	bool IsOutsideAttackRadius();
	//�Ƿ��ڹ�����Χ��
	bool IsInsideAttackRadius();
	//�Ƿ���Ѳ��״̬
	bool IsChasing();
	//�Ƿ��ڹ���״̬
	bool IsAttacking();
	//�Ƿ�������״̬
	bool IsDead();
	//�Ƿ���æµ״̬
	bool IsEngaged();
	//���Ѳ�߼�ʱ��
	void ClearPatrolTimer();
	//���ù�����ʱ��
	void StartAttackTimer();
	//���������ʱ��
	void ClearAttackTimer();
	//���Ŀ���Ƿ���ս����Χ��
	bool InTargetRange(AActor* Target, double Radius);
	//�ƶ���Ŀ��
	void MoveToTarget(AActor* Target);
	//ѡ���µ�Ѳ�ߵ�
	AActor* ChoosePatrolTarget();
	//����Ĭ������
	void SpawnDefaultWeapon();
	/*------------------------------*/

	/*----------�ص�����----------*/
	//Pawn��֪�ص�����
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	////AI��֪�ص�����
	//UFUNCTION()
	//void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	/*----------------------------*/

	/*----------���----------*/
	//Ѫ�����
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthbarComponent> HealthBarWidget;

	//Pawn��֪���
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensing;

	////AI��֪���
	//UPROPERTY(VisibleAnywhere, Category = "AI")
	//TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	//UPROPERTY(VisibleAnywhere, Category = "AI")
	//TObjectPtr<UAISenseConfig_Sight> SightConfig;

	//UPROPERTY(VisibleAnywhere, Category = "AI")
	//TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	/*------------------------*/

	/*----------�������----------*/
	//AI������
	UPROPERTY()
	TObjectPtr<AAIController> EnemyController;

	//��ǰѲ�ߵ�
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	//Ѳ�ߵ�����
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets;

	//Ѳ�߰뾶
	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrolRadius = 200.f;

	//Ѳ�ߵȴ�ʱ��
	FTimerHandle PatrolTimer;

	//��С�����ȴ�ʱ��
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	//Ѳ���ٶ�
	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrollingSpeed = 125.f;
	/*----------------------------*/

	/*----------ս�����----------*/
	//��������
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<ABaseWeapon> WeaponClass;

	//ս���뾶
	UPROPERTY(EditAnywhere, Category = "Combat")
	float CombatRadius = 1500.f;

	//������Χ
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRadius = 200.f;

	//���ܰ뾶	
	UPROPERTY(EditAnywhere, Category = "Combat")
	double AcceptanceRadius = 100.f;

	//������ʱ��
	FTimerHandle AttackTimer;

	//��С����󹥻����ʱ��
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;

	//׷���ٶ�
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 300.f;

	//�����ӳ�����ʱ��
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.f;

	//�����
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class ASoul> SoulClass;
	/*----------------------------*/
};
