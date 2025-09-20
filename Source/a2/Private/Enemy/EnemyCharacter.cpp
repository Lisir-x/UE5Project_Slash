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
 	//����Ϊfalseʱ��ֹͣTick�¼�
	PrimaryActorTick.bCanEverTick = true;

	//������������ײ����
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);	//�������ص��¼�

	//�����ƶ��������
	GetCharacterMovement()->bOrientRotationToMovement = true;	//�����ƶ�����
	//��ɫ�������������ת
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//����Ѫ�����
	HealthBarWidget = CreateDefaultSubobject<UHealthbarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(RootComponent);

	//����Pawn��֪���
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	//����Pawn��֪�������
	PawnSensing->HearingThreshold = 600.f;		//������ֵ
	PawnSensing->LOSHearingThreshold = 1200.f;	//����������ֵ
	PawnSensing->SightRadius = 3000.f;			//�Ӿ�
	PawnSensing->SetPeripheralVisionAngle(60.f);//��Ұ�Ƕ�
	PawnSensing->SensingInterval = 0.1f;		//��֪���
	PawnSensing->bOnlySensePlayers = true;		//ֻ��֪���

	////����AI��֪���
	//PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	////�Ӿ���֪����
	//SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	//SightConfig->SightRadius = 3000.f;                //�Ӿ�
	//SightConfig->PeripheralVisionAngleDegrees = 60.f; //��Ұ�Ƕ�
	//SightConfig->SetMaxAge(0.5f);					  //��֪���
	//SightConfig->DetectionByAffiliation.bDetectEnemies = true;//ֻ��֪���
	////������֪����
	//HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	//HearingConfig->HearingRange = 600.f;          //������Χ
	//HearingConfig->SetMaxAge(0.5f);               //��֪���
	//HearingConfig->DetectionByAffiliation.bDetectEnemies = true;//ֻ��֪���
	////����֪���ð󶨵���֪���
	//PerceptionComponent->ConfigureSense(*SightConfig);
	//PerceptionComponent->ConfigureSense(*HearingConfig);
	////����������֪����Ϊ�Ӿ�
	//PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

//ÿһ֡������
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//���������ֱ�ӷ���
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling)//�������׷��򹥻�״̬
	{
		//ѡ��ս��Ŀ��
		CheckCombatTarget();
	}
	else //�������Ѳ��״̬
	{
		//ѡ��Ѳ��Ŀ��
		CheckPatrolTarget();
	}
}

//���˺���
float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	//�����˺�
	HandleDamage(DamageAmount);
	//���õ�ǰս��Ŀ��Ϊ�˺�ִ����
	CombatTarget = EventInstigator->GetPawn();

	if (IsInsideAttackRadius())	//����ڹ�����Χ��
	{
		//���õ���״̬Ϊ����
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius()) //����ڹ�����Χ��
	{
		//׷��Ŀ��
		ChaseTarget();
	}

	return DamageAmount;
}

//����ʱ����
void AEnemyCharacter::Destroyed()
{
	if (EquippedWeapon)
	{
		//������װ��������
		EquippedWeapon->Destroy();
	}
}

//���к������ܵ��˺�ʱ����
void AEnemyCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	//������δ��������ʾѪ��
	if (!IsDead()) ShowHealthBar();
	//���Ѳ�߼�ʱ��
	ClearPatrolTimer();
	//���������ʱ��
	ClearAttackTimer();
	//����������ײ
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	//�жϹ�������
	StopAttackMontage();

	if (IsInsideAttackRadius())
	{
		if (!IsDead()) StartAttackTimer();
	}
}

//��Ϸ��ʼ������ʱ����
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//��Pawn��֪������¼�
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyCharacter::PawnSeen);

	////��AI��֪������¼�
	//if (PerceptionComponent)
	//{
	//	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::OnPerceptionUpdated);
	//}

	//��ʼ������
	InitializeEnemy();

	//��ӵ��˱�ǩ
	Tags.Add(FName("Enemy"));
}

//�����߼�
void AEnemyCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	if (EnemyController)
	{
		EnemyController->StopMovement();   //ֹͣ PathFollowing
		EnemyController->ClearFocus(EAIFocusPriority::Gameplay); //���� Focus
	}
	//������ж�ʱ��
	GetWorldTimerManager().ClearAllTimersForObject(this);
	if (PawnSensing)
	{
		//���Pawn��֪������¼�
		PawnSensing->OnSeePawn.RemoveDynamic(this, &AEnemyCharacter::PawnSeen);
	}
	//�������Ŀ�꣬���� MoveTo ����
	CombatTarget = nullptr;

	//���õ���״̬Ϊ����
	EnemyState = EEnemyState::EES_Dead;
	//����Ѫ��
	HideHealthBar();
	//���ý�������ײ
	DisableCapsule();
	//������������ײ
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//����������ײ
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	//���ٽ�ɫ
	SetLifeSpan(DeathLifeSpan);
	//�������
	SpawnSoul();
}

//�������
void AEnemyCharacter::SpawnSoul()
{
	//��ȡ����������
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes)
	{
		//ָ������λ��
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		//��ָ��λ���������
		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (SpawnedSoul)
		{
			//��ȡ�������������
			SpawnedSoul->SetSouls(Attributes->GetSouls());
		}
	}
}

//��������
void AEnemyCharacter::Attack()
{
	Super::Attack();
	//���û��ս��Ŀ����ֱ�ӷ���
	if (!CombatTarget) return;
	//���õ���״̬Ϊæµ
	EnemyState = EEnemyState::EES_Engaged;
	//���ŵ��˹�����̫��
	PlayAttackMontage();
}

//�Ƿ���Թ���
bool AEnemyCharacter::CanAttack()
{
	return IsInsideAttackRadius()
		&& !IsAttacking()
		&& !IsEngaged()
		&& !IsDead();
}

//��������
void AEnemyCharacter::AttackEnd()
{
	//����״̬����Ϊ��״̬
	EnemyState = EEnemyState::EES_NoState;
	//ѡ��ս��Ŀ��
	CheckCombatTarget();
}

//�����˺�
void AEnemyCharacter::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarWidget)
	{
		//����Ѫ�������Ѫ���ٷֱ�
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

//��ʼ������
void AEnemyCharacter::InitializeEnemy()
{
	//��ȡAI������
	EnemyController = Cast<AAIController>(GetController());
	//AI�ƶ���Ŀ��Ѳ�ߵ�
	MoveToTarget(PatrolTarget);
	//��ʼ��Ѫ�������Ѫ���ٷֱ�
	HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	//��ʼʱ����Ѫ��
	HideHealthBar();
	//����Ĭ������
	SpawnDefaultWeapon();
}

//ѡ��Ѳ��Ŀ��
void AEnemyCharacter::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius)) //�����Ѳ�߷�Χ��
	{
		//ѡ���µ�Ѳ�ߵ�
		PatrolTarget = ChoosePatrolTarget();
		//����ȴ�ʱ��
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		//����Ѳ�߼�ʱ��
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyCharacter::PatrolTimerFinished, WaitTime);
	}
}

//ѡ��ս��Ŀ��
void AEnemyCharacter::CheckCombatTarget()
{
	if (IsOutsideCombatRadius()) //�������ս����Χ��
	{
		//���������ʱ��
		ClearAttackTimer();
		//ʧȥ��Ȥ
		LoseInterest();
		//���������æµ״̬����ʼѲ��
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing()) //�ڹ�����Χ��
	{
		//���������ʱ��
		ClearAttackTimer();
		//���������æµ״̬��׷��Ŀ��
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())	//�ɹ���
	{
		//���ù�����ʱ��
		StartAttackTimer();
	}
}

//Ѳ�ߵȴ�ʱ��ص�����
void AEnemyCharacter::PatrolTimerFinished()
{
	//AI�ƶ���Ŀ��Ѳ�ߵ�
	MoveToTarget(PatrolTarget);
}

//����Ѫ��
void AEnemyCharacter::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

//��ʾѪ��
void AEnemyCharacter::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

//ʧȥ��Ȥ
void AEnemyCharacter::LoseInterest()
{
	//���ս��Ŀ��
	CombatTarget = nullptr;
	//����Ѫ��
	HideHealthBar();
}

//��ʼѲ��
void AEnemyCharacter::StartPatrolling()
{
	//���õ���״̬ΪѲ��
	EnemyState = EEnemyState::EES_Patrolling;
	//����Ѳ��ʱ���ƶ��ٶ�
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	//ѡ���µ�Ѳ�ߵ�
	PatrolTarget = ChoosePatrolTarget();
	//AI�ƶ���Ŀ��Ѳ�ߵ�
	MoveToTarget(PatrolTarget);
}

//׷��Ŀ��
void AEnemyCharacter::ChaseTarget()
{
	//���õ���״̬Ϊ׷��
	EnemyState = EEnemyState::EES_Chasing;
	//����׷��ʱ���ƶ��ٶ�
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	//AI�ƶ���ս��Ŀ��
	MoveToTarget(CombatTarget);
}

//�Ƿ񳬳�ս����Χ
bool AEnemyCharacter::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}
//�Ƿ񳬳�������Χ
bool AEnemyCharacter::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}
//�Ƿ��ڹ�����Χ��
bool AEnemyCharacter::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}
//�Ƿ���Ѳ��״̬
bool AEnemyCharacter::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}
//�Ƿ��ڹ���״̬
bool AEnemyCharacter::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}
//�Ƿ�������״̬
bool AEnemyCharacter::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}
//�Ƿ���æµ״̬�������򱻻���
bool AEnemyCharacter::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

//���Ѳ�߼�ʱ��
void AEnemyCharacter::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

//���ù�����ʱ��
void AEnemyCharacter::StartAttackTimer()
{
	//���õ���״̬Ϊ����
	EnemyState = EEnemyState::EES_Attacking;
	//������ɹ������ʱ��
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	//���ù�����ʱ��
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyCharacter::Attack, AttackTime);
}
//���������ʱ��
void AEnemyCharacter::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

//���Ŀ���Ƿ���ս����Χ��
bool AEnemyCharacter::InTargetRange(AActor* Target, double Radius)
{
	if (!Target) return false;
	////���Ƶ�����
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
	//const float Distance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
	//return Distance <= Radius;
	return FVector::Distance(Target->GetActorLocation(), GetActorLocation()) <= Radius;
}

//�ƶ���Ŀ��
void AEnemyCharacter::MoveToTarget(AActor* Target)
{
	if (!EnemyController || !Target) return;
	//����AI�ƶ�����
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);	  //����Ŀ��
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);//���ý��ܰ뾶
	//ִ���ƶ�����
	EnemyController->MoveTo(MoveRequest);
}

//ѡ���µ�Ѳ�ߵ�
AActor* AEnemyCharacter::ChoosePatrolTarget()
{
	//��ЧѲ�ߵ���
	TArray<AActor*> ValidTargets; //�洢������ЧѲ�ߵ�
	for (AActor* Target : PatrolTargets)
	{
		if (Target && Target != PatrolTarget) //�ų���ָ��͵�ǰѲ�ߵ�
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumTargets = ValidTargets.Num(); //��ȡѲ�ߵ�����
	if (NumTargets > 0)
	{
		const int32 Selection = FMath::RandRange(0, NumTargets - 1); //���ѡ��һ��Ѳ�ߵ�����
		return ValidTargets[Selection];		//�����µ�Ѳ�ߵ�
	}
	//û����ЧѲ�ߵ��򷵻ؿ�ָ��
	return nullptr;
}

//����Ĭ������
void AEnemyCharacter::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		//��������
		ABaseWeapon* DefaultWeapon = World->SpawnActor<ABaseWeapon>(WeaponClass);
		//װ������
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;

	}
}

//Pawn��֪�������Pawnʱ����
void AEnemyCharacter::PawnSeen(APawn* SeenPawn)
{
	//�ж��Ƿ�Ӧ��׷��Ŀ��
	const bool bShouldChaseTarget =
		//EnemyState != EEnemyState::EES_Dead &&
		//EnemyState != EEnemyState::EES_Chasing &&
		//EnemyState < EEnemyState::EES_Attacking &&
		EnemyState == EEnemyState::EES_Patrolling &&
		SeenPawn->ActorHasTag(FName("EngageableTarget"));
	
	if (bShouldChaseTarget)
	{
		//��ʾѪ��
		ShowHealthBar();
		//����ս��Ŀ��
		CombatTarget = SeenPawn;
		//���Ѳ�߶�ʱ��
		ClearPatrolTimer();
		//׷��Ŀ��
		ChaseTarget();
	}

}
