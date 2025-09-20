// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Component/AttributeComponent.h"
#include "HUD/HealthbarComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Items/Soul.h"
//#include "Perception/AIPerceptionComponent.h"
//#include "Perception/AISenseConfig_Sight.h"
//#include "Perception/AISenseConfig_Hearing.h"

AEnemyCharacter::AEnemyCharacter()
{
 	//当设为false时将停止Tick事件
	PrimaryActorTick.bCanEverTick = true;

	//设置网格体碰撞属性
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);	//可生成重叠事件

	//设置移动组件属性
	GetCharacterMovement()->bOrientRotationToMovement = true;	//朝向移动方向
	//角色不跟随控制器旋转
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//创建血条组件
	HealthBarWidget = CreateDefaultSubobject<UHealthbarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(RootComponent);

	//创建Pawn感知组件
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	//设置Pawn感知组件属性
	PawnSensing->HearingThreshold = 600.f;		//听觉阈值
	PawnSensing->LOSHearingThreshold = 1200.f;	//视线听觉阈值
	PawnSensing->SightRadius = 3000.f;			//视距
	PawnSensing->SetPeripheralVisionAngle(60.f);//视野角度
	PawnSensing->SensingInterval = 0.1f;		//感知间隔
	PawnSensing->bOnlySensePlayers = true;		//只感知玩家

	////创建AI感知组件
	//PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	////视觉感知配置
	//SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	//SightConfig->SightRadius = 3000.f;                //视距
	//SightConfig->PeripheralVisionAngleDegrees = 60.f; //视野角度
	//SightConfig->SetMaxAge(0.5f);					  //感知间隔
	//SightConfig->DetectionByAffiliation.bDetectEnemies = true;//只感知玩家
	////听觉感知配置
	//HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	//HearingConfig->HearingRange = 600.f;          //听觉范围
	//HearingConfig->SetMaxAge(0.5f);               //感知间隔
	//HearingConfig->DetectionByAffiliation.bDetectEnemies = true;//只感知玩家
	////将感知配置绑定到感知组件
	//PerceptionComponent->ConfigureSense(*SightConfig);
	//PerceptionComponent->ConfigureSense(*HearingConfig);
	////设置主导感知类型为视觉
	//PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

//每一帧都调用
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//如果死亡则直接返回
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling)//如果处于追逐或攻击状态
	{
		//选择战斗目标
		CheckCombatTarget();
	}
	else //如果处于巡逻状态
	{
		//选择巡逻目标
		CheckPatrolTarget();
	}
}

//受伤函数
float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	//处理伤害
	HandleDamage(DamageAmount);
	//设置当前战斗目标为伤害执行者
	CombatTarget = EventInstigator->GetPawn();

	if (IsInsideAttackRadius())	//如果在攻击范围内
	{
		//设置敌人状态为攻击
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius()) //如果在攻击范围外
	{
		//追逐目标
		ChaseTarget();
	}

	return DamageAmount;
}

//销毁时调用
void AEnemyCharacter::Destroyed()
{
	if (EquippedWeapon)
	{
		//销毁已装备的武器
		EquippedWeapon->Destroy();
	}
}

//命中函数，受到伤害时调用
void AEnemyCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	//若敌人未死亡则显示血条
	if (!IsDead()) ShowHealthBar();
	//清除巡逻计时器
	ClearPatrolTimer();
	//清除攻击计时器
	ClearAttackTimer();
	//禁用武器碰撞
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	//中断攻击动画
	StopAttackMontage();

	if (IsInsideAttackRadius())
	{
		if (!IsDead()) StartAttackTimer();
	}
}

//游戏开始或生成时调用
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//绑定Pawn感知组件的事件
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyCharacter::PawnSeen);

	////绑定AI感知组件的事件
	//if (PerceptionComponent)
	//{
	//	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::OnPerceptionUpdated);
	//}

	//初始化敌人
	InitializeEnemy();

	//添加敌人标签
	Tags.Add(FName("Enemy"));
}

//死亡逻辑
void AEnemyCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	if (EnemyController)
	{
		EnemyController->StopMovement();   //停止 PathFollowing
		EnemyController->ClearFocus(EAIFocusPriority::Gameplay); //清理 Focus
	}
	//清除所有定时器
	GetWorldTimerManager().ClearAllTimersForObject(this);
	if (PawnSensing)
	{
		//解绑Pawn感知组件的事件
		PawnSensing->OnSeePawn.RemoveDynamic(this, &AEnemyCharacter::PawnSeen);
	}
	//立即清空目标，避免 MoveTo 再用
	CombatTarget = nullptr;

	//设置敌人状态为死亡
	EnemyState = EEnemyState::EES_Dead;
	//隐藏血条
	HideHealthBar();
	//禁用胶囊体碰撞
	DisableCapsule();
	//禁用网格体碰撞
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//禁用武器碰撞
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	//销毁角色
	SetLifeSpan(DeathLifeSpan);
	//生成灵魂
	SpawnSoul();
}

//生成灵魂
void AEnemyCharacter::SpawnSoul()
{
	//获取世界上下文
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes)
	{
		//指定生成位置
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		//在指定位置生成灵魂
		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (SpawnedSoul)
		{
			//获取并设置灵魂数量
			SpawnedSoul->SetSouls(Attributes->GetSouls());
		}
	}
}

//攻击函数
void AEnemyCharacter::Attack()
{
	Super::Attack();
	//如果没有战斗目标则直接返回
	if (!CombatTarget) return;
	//设置敌人状态为忙碌
	EnemyState = EEnemyState::EES_Engaged;
	//播放敌人攻击蒙太奇
	PlayAttackMontage();
}

//是否可以攻击
bool AEnemyCharacter::CanAttack()
{
	return IsInsideAttackRadius()
		&& !IsAttacking()
		&& !IsEngaged()
		&& !IsDead();
}

//攻击结束
void AEnemyCharacter::AttackEnd()
{
	//敌人状态重置为无状态
	EnemyState = EEnemyState::EES_NoState;
	//选择战斗目标
	CheckCombatTarget();
}

//处理伤害
void AEnemyCharacter::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarWidget)
	{
		//更新血条组件的血量百分比
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

//初始化敌人
void AEnemyCharacter::InitializeEnemy()
{
	//获取AI控制器
	EnemyController = Cast<AAIController>(GetController());
	//AI移动到目标巡逻点
	MoveToTarget(PatrolTarget);
	//初始化血条组件的血量百分比
	HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	//初始时隐藏血条
	HideHealthBar();
	//生成默认武器
	SpawnDefaultWeapon();
}

//选择巡逻目标
void AEnemyCharacter::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius)) //如果在巡逻范围内
	{
		//选择新的巡逻点
		PatrolTarget = ChoosePatrolTarget();
		//随机等待时间
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		//设置巡逻计时器
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyCharacter::PatrolTimerFinished, WaitTime);
	}
}

//选择战斗目标
void AEnemyCharacter::CheckCombatTarget()
{
	if (IsOutsideCombatRadius()) //如果不在战斗范围内
	{
		//清除攻击计时器
		ClearAttackTimer();
		//失去兴趣
		LoseInterest();
		//如果不处于忙碌状态，开始巡逻
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing()) //在攻击范围外
	{
		//清除攻击计时器
		ClearAttackTimer();
		//如果不处于忙碌状态，追逐目标
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())	//可攻击
	{
		//启用攻击计时器
		StartAttackTimer();
	}
}

//巡逻等待时间回调函数
void AEnemyCharacter::PatrolTimerFinished()
{
	//AI移动到目标巡逻点
	MoveToTarget(PatrolTarget);
}

//隐藏血条
void AEnemyCharacter::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

//显示血条
void AEnemyCharacter::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

//失去兴趣
void AEnemyCharacter::LoseInterest()
{
	//清除战斗目标
	CombatTarget = nullptr;
	//隐藏血条
	HideHealthBar();
}

//开始巡逻
void AEnemyCharacter::StartPatrolling()
{
	//设置敌人状态为巡逻
	EnemyState = EEnemyState::EES_Patrolling;
	//设置巡逻时的移动速度
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	//选择新的巡逻点
	PatrolTarget = ChoosePatrolTarget();
	//AI移动到目标巡逻点
	MoveToTarget(PatrolTarget);
}

//追逐目标
void AEnemyCharacter::ChaseTarget()
{
	//设置敌人状态为追逐
	EnemyState = EEnemyState::EES_Chasing;
	//设置追逐时的移动速度
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	//AI移动到战斗目标
	MoveToTarget(CombatTarget);
}

//是否超出战斗范围
bool AEnemyCharacter::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}
//是否超出攻击范围
bool AEnemyCharacter::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}
//是否在攻击范围内
bool AEnemyCharacter::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}
//是否处于巡逻状态
bool AEnemyCharacter::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}
//是否处于攻击状态
bool AEnemyCharacter::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}
//是否处于死亡状态
bool AEnemyCharacter::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}
//是否处于忙碌状态（攻击或被击）
bool AEnemyCharacter::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

//清除巡逻计时器
void AEnemyCharacter::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

//启用攻击计时器
void AEnemyCharacter::StartAttackTimer()
{
	//设置敌人状态为攻击
	EnemyState = EEnemyState::EES_Attacking;
	//随机生成攻击间隔时间
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	//设置攻击计时器
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyCharacter::Attack, AttackTime);
}
//清除攻击计时器
void AEnemyCharacter::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

//检查目标是否在战斗范围内
bool AEnemyCharacter::InTargetRange(AActor* Target, double Radius)
{
	if (!Target) return false;
	////绘制调试球
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
	//const float Distance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
	//return Distance <= Radius;
	return FVector::Distance(Target->GetActorLocation(), GetActorLocation()) <= Radius;
}

//移动到目标
void AEnemyCharacter::MoveToTarget(AActor* Target)
{
	if (!EnemyController || !Target) return;
	//创建AI移动请求
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);	  //设置目标
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);//设置接受半径
	//执行移动命令
	EnemyController->MoveTo(MoveRequest);
}

//选择新的巡逻点
AActor* AEnemyCharacter::ChoosePatrolTarget()
{
	//有效巡逻点检测
	TArray<AActor*> ValidTargets; //存储所有有效巡逻点
	for (AActor* Target : PatrolTargets)
	{
		if (Target && Target != PatrolTarget) //排除空指针和当前巡逻点
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumTargets = ValidTargets.Num(); //获取巡逻点数量
	if (NumTargets > 0)
	{
		const int32 Selection = FMath::RandRange(0, NumTargets - 1); //随机选择一个巡逻点索引
		return ValidTargets[Selection];		//返回新的巡逻点
	}
	//没有有效巡逻点则返回空指针
	return nullptr;
}

//生成默认武器
void AEnemyCharacter::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		//生成武器
		ABaseWeapon* DefaultWeapon = World->SpawnActor<ABaseWeapon>(WeaponClass);
		//装备武器
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;

	}
}

//Pawn感知组件看到Pawn时调用
void AEnemyCharacter::PawnSeen(APawn* SeenPawn)
{
	//判断是否应该追逐目标
	const bool bShouldChaseTarget =
		//EnemyState != EEnemyState::EES_Dead &&
		//EnemyState != EEnemyState::EES_Chasing &&
		//EnemyState < EEnemyState::EES_Attacking &&
		EnemyState == EEnemyState::EES_Patrolling &&
		SeenPawn->ActorHasTag(FName("EngageableTarget"));
	
	if (bShouldChaseTarget)
	{
		//显示血条
		ShowHealthBar();
		//设置战斗目标
		CombatTarget = SeenPawn;
		//清除巡逻定时器
		ClearPatrolTimer();
		//追逐目标
		ChaseTarget();
	}

}
