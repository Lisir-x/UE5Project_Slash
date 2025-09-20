// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Component/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	//����Ϊfalseʱ��ֹͣTick�¼�
	PrimaryActorTick.bCanEverTick = true;

	//�����������
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute"));

	//���ý�������ײ����
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);//����Cameraͨ��

}

//��Ϸ��ʼ������ʱ����
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

//���к������ܵ��˺�ʱ����
void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	//����������ܻ���Ӧ
	if (IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	else Die();	//������������߼�

	//�����ܻ���Ч������Ч��
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

//�����߼�
void ABaseCharacter::Attack()
{
	//���ս��Ŀ�����������ÿ�ս��Ŀ��
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead"))) CombatTarget = nullptr;
}

//�����߼�
void ABaseCharacter::Die_Implementation()
{
	//���������ǩ
	Tags.Add(FName("Dead"));
	//����������̫��
	PlayDeathMontage();
}

//�����ܻ���̫��
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	//��ȡ����ʵ��
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		//����װ������
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);//��ת��ָ����
	}
}

//�������ܻ���Ӧ
void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	//��ȡǰ������
	const FVector Forward = GetActorForwardVector();
	//�����ܻ�ˮƽ������������һ��
	const FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal2D();
	//����Ƕ�
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, ToHit)));
	//����������ˣ������ж�����
	const FVector Cross = FVector::CrossProduct(Forward, ToHit);
	//����UE5���ֶ���Cross����0��Ϊ���֣�����Ϊ����
	if (Cross.Z < 0) Angle *= -1.f;

	//���ݽǶ��жϴ��ĸ������ܻ�
	FName Sction("FromBack");	//Ĭ�ϴӺ��ܻ�
	if (Angle >= -45.f && Angle < 45.f) Sction = "FromFront";
	else if (Angle <= -135.f && Angle < -45.f) Sction = "FromLeft";
	else if (Angle >= 45.f && Angle < 135.f) Sction = "FromRight";
	//�����ܻ�����
	PlayHitReactMontage(Sction);

}

//�����ܻ���Ч
void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

//�����ܻ�����Ч��
void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{

	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

//�����˺�
void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		//�������˽ӿں���
		Attributes->ReceiveDamage(DamageAmount);
	}
}

//����ָ����̫���ָ����
void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

//���������̫���
int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;	//û�ж������򷵻�-1
	const int32 MaxSectionIndex = SectionNames.Num() - 1;	//��������
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);//���ѡ��һ��������
	PlayMontageSection(Montage, SectionNames[Selection]);	//����ѡ��Ķ�
	return Selection;
}

//���Ź�����̫��
int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

//����������̫��
int32 ABaseCharacter::PlayDeathMontage()
{
	//�������������̫��β�������������
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	//ȷ��������ö�ٷ�Χ��
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	//����ѡ��Ķ�����
	return Selection;
}

//�жϹ�����̫��
void ABaseCharacter::StopAttackMontage()
{
	//��ȡ����ʵ��
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		//ֹͣ��������
		AnimInstance->Montage_Stop(0.2f, AttackMontage);
	}
}

//����������̫��
void ABaseCharacter::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, FName("Default"));
}

//��ȡŤ��Ŀ��λ��
FVector ABaseCharacter::GetTranslationWarpTarget()
{
	//���û��ս��Ŀ���򷵻�������
	if (CombatTarget == nullptr) return FVector();
	//��ȡĿ��������λ��
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	//����Ŀ��㵽����ķ�����������һ����Ȼ�����Ť������
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	//����Ŀ�����Ϸ�������
	return CombatTargetLocation + TargetToMe;
}

//��ȡŤ��Ŀ����ת
FVector ABaseCharacter::GetRotationWarpTarget()
{
	//������ս��Ŀ���򷵻���λ�ã����򷵻�������
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

//���ý�������ײ
void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

//�ж��Ƿ���Թ���
bool ABaseCharacter::CanAttack()
{
	return false;
}

//�ж��Ƿ���
bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

//��������
void ABaseCharacter::AttackEnd()
{
}

//���ܽ���
void ABaseCharacter::DodgeEnd()
{
}

//ÿһ֡������
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//����������ײ����
void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		//����������ײ�����ײ��Ӧ����
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnable);
		//��պ����б�
		EquippedWeapon->IgnoreActors.Empty();
	}
}

//���ù�����������ײ����
void ABaseCharacter::SetMeshCollisionResponses(
	const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Responses)
{
	for (const auto& Elem : Responses)
	{
		//���ù������������ײ��Ӧ����
		ECollisionChannel Channel = Elem.Key;	//��ײͨ��
		ECollisionResponse Response = Elem.Value; //��ײ��Ӧ����
		GetMesh()->SetCollisionResponseToChannel(Channel, Response);
	}
}


