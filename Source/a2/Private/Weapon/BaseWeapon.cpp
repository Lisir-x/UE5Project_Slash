

#include "Weapon/BaseWeapon.h"
#include "Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"


ABaseWeapon::ABaseWeapon()
{
	//创建碰撞盒组件默认子对象
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(ItemMesh);
	//设置碰撞盒组件的碰撞属性
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);//无碰撞
	WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);//世界动态物体
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);//与所有通道重叠
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);//忽略Pawn通道

	//创建BoxTraceStart组件默认子对象
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(ItemMesh);

	//创建BoxTraceEnd组件默认子对象
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(ItemMesh);
}

//装备武器
void ABaseWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigetor)
{

	//设置物品状态为装备
	ItemState = EItemState::EIS_Equipped;
	//设置武器的拥有者和执行者
	SetOwner(NewOwner);
	SetInstigator(NewInstigetor);
	//将武器网格附加到指定组件的指定插槽
	AttachMeshToSocket(InParent, InSocketName);
	//禁用碰撞球体
	DisableSphereCollision();
	//播放装备音效
	PlayEquipSound();
	//禁用粒子系统组件
	DeactivateEmbers();
}

//禁用碰撞球体
void ABaseWeapon::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//播放装备音效
void ABaseWeapon::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
}

//禁用粒子系统组件
void ABaseWeapon::DeactivateEmbers()
{
	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

//将武器网格附加到指定组件的指定插槽
void ABaseWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	//定义附加规则：
	//参数1：将武器网格的位置和旋转设置为插槽的位置和旋转，缩放保持不变
	//参数2：是否保持相对位置（焊接物理体）
	FAttachmentTransformRules TransformRule(EAttachmentRule::SnapToTarget, true);
	//将武器网格附加到玩家骨骼网格体
	//FAttachmentTransformRules::SnapToTargetIncludingScale替换TransformRule：
	//将武器网格的位置、旋转和缩放都设置为插槽的位置、旋转和缩放，不焊接物理体
	ItemMesh->AttachToComponent(InParent, TransformRule, InSocketName);
}

//游戏开始或生成时调用
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	//绑定回调函数到基本组件委托
	//武器碰撞盒重叠事件
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnBoxOverlap);
}

//武器碰撞盒重叠事件
void ABaseWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	//如果重叠的Actor是同类，则直接返回
	if (ActorIsSameType(OtherActor)) return;

	//存储碰撞检测结果的结构体
	FHitResult BoxHit;
	//盒形追踪
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())	//如果有碰撞
	{
		//如果碰撞到的Actor是同类，则直接返回
		if (ActorIsSameType(BoxHit.GetActor())) return;
		//应用伤害
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),	//受击Actor
			Damage,				//伤害值
			GetInstigator()->GetController(),//造成伤害的控制器
			this,				//造成伤害的Actor
			UDamageType::StaticClass()	//伤害类型
		);
		//执行命中检测
		ExecuteGetHit(BoxHit);
		//调用创建力场函数
		CreateFields(BoxHit.ImpactPoint);

	}
}

//判断是否为相同类型的Actor
bool ABaseWeapon::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

//执行命中检测
void ABaseWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	//尝试将碰撞到的Actor转换为命中接口（判断目标是否实现了受击逻辑）
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)	//如果实现了受击接口
	{
		//调用受击接口的受击函数
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

//盒形追踪
void ABaseWeapon::BoxTrace(FHitResult& BoxHit)
{
	//获取追踪组件位置
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	//创建需要忽略的Actor数组（避免检测自身）
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);//将当前武器自身加入忽略列表
	ActorsToIgnore.Add(GetOwner());//将武器的拥有者加入忽略列表
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);//将忽略列表中的Actor加入忽略列表
	}

	//执行盒体碰撞检测
	UKismetSystemLibrary::BoxTraceSingle(
		this, Start, End,	//对象、起点、终点
		BoxTraceExtent,		//盒体半尺寸（三维向量）
		BoxTraceStart->GetComponentRotation(), //使用指定组件的旋转方向
		ETraceTypeQuery::TraceTypeQuery1, //使用的碰撞通道类型
		false,	//不检测复杂碰撞（设为true可检测三角形级别的碰撞）
		ActorsToIgnore, //需要忽略的Actor数组
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,//调试绘制模式
		BoxHit,	//输出参数，接收碰撞结果
		true	//是否忽略自身（与ActorToIgnore配合使用）
	);
	//将碰撞到的Actor加入忽略列表
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
