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
 	//当设为false时将停止Tick事件
	PrimaryActorTick.bCanEverTick = true;

	//创建属性组件
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute"));

	//设置胶囊体碰撞属性
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);//忽略Camera通道

}

//游戏开始或生成时调用
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

//命中函数，受到伤害时调用
void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	//如果存活，调用受击反应
	if (IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	else Die();	//否则调用死亡逻辑

	//播放受击音效和粒子效果
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

//攻击逻辑
void ABaseCharacter::Attack()
{
	//如果战斗目标已死亡则置空战斗目标
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead"))) CombatTarget = nullptr;
}

//死亡逻辑
void ABaseCharacter::Die_Implementation()
{
	//添加死亡标签
	Tags.Add(FName("Dead"));
	//播放死亡蒙太奇
	PlayDeathMontage();
}

//播放受击蒙太奇
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	//获取动画实例
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		//播放装备动画
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);//跳转到指定段
	}
}

//方向性受击反应
void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	//获取前向向量
	const FVector Forward = GetActorForwardVector();
	//计算受击水平方向向量并归一化
	const FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal2D();
	//计算角度
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, ToHit)));
	//计算向量叉乘，用来判断左右
	const FVector Cross = FVector::CrossProduct(Forward, ToHit);
	//根据UE5左手定则，Cross大于0则为右手，否则为左手
	if (Cross.Z < 0) Angle *= -1.f;

	//根据角度判断从哪个方向受击
	FName Sction("FromBack");	//默认从后方受击
	if (Angle >= -45.f && Angle < 45.f) Sction = "FromFront";
	else if (Angle <= -135.f && Angle < -45.f) Sction = "FromLeft";
	else if (Angle >= 45.f && Angle < 135.f) Sction = "FromRight";
	//播放受击动画
	PlayHitReactMontage(Sction);

}

//播放受击音效
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

//播放受击粒子效果
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

//处理伤害
void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		//调用受伤接口函数
		Attributes->ReceiveDamage(DamageAmount);
	}
}

//播放指定蒙太奇的指定段
void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

//随机播放蒙太奇段
int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;	//没有段名称则返回-1
	const int32 MaxSectionIndex = SectionNames.Num() - 1;	//最大段索引
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);//随机选择一个段索引
	PlayMontageSection(Montage, SectionNames[Selection]);	//播放选择的段
	return Selection;
}

//播放攻击蒙太奇
int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

//播放死亡蒙太奇
int32 ABaseCharacter::PlayDeathMontage()
{
	//随机播放死亡蒙太奇段并设置死亡姿势
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	//确保姿势在枚举范围内
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	//返回选择的段索引
	return Selection;
}

//中断攻击蒙太奇
void ABaseCharacter::StopAttackMontage()
{
	//获取动画实例
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		//停止攻击动画
		AnimInstance->Montage_Stop(0.2f, AttackMontage);
	}
}

//播放闪避蒙太奇
void ABaseCharacter::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, FName("Default"));
}

//获取扭曲目标位移
FVector ABaseCharacter::GetTranslationWarpTarget()
{
	//如果没有战斗目标则返回零向量
	if (CombatTarget == nullptr) return FVector();
	//获取目标点和自身位置
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	//计算目标点到自身的方向向量并归一化，然后乘以扭曲距离
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	//返回目标点加上方向向量
	return CombatTargetLocation + TargetToMe;
}

//获取扭曲目标旋转
FVector ABaseCharacter::GetRotationWarpTarget()
{
	//若存在战斗目标则返回其位置，否则返回零向量
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

//禁用胶囊体碰撞
void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

//判断是否可以攻击
bool ABaseCharacter::CanAttack()
{
	return false;
}

//判断是否存活
bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

//攻击结束
void ABaseCharacter::AttackEnd()
{
}

//闪避结束
void ABaseCharacter::DodgeEnd()
{
}

//每一帧都调用
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//设置武器碰撞类型
void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		//设置武器碰撞体的碰撞响应类型
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnable);
		//清空忽略列表
		EquippedWeapon->IgnoreActors.Empty();
	}
}

//设置骨骼网格体碰撞属性
void ABaseCharacter::SetMeshCollisionResponses(
	const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Responses)
{
	for (const auto& Elem : Responses)
	{
		//设置骨骼网格体的碰撞响应类型
		ECollisionChannel Channel = Elem.Key;	//碰撞通道
		ECollisionResponse Response = Elem.Value; //碰撞响应类型
		GetMesh()->SetCollisionResponseToChannel(Channel, Response);
	}
}



