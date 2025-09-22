// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "a2/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AItem::AItem()
{
 	//当设为false时将停止Tick事件
	PrimaryActorTick.bCanEverTick = true;

	//创建基本组件
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//创建静态网格体组件
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	//设置静态网格体碰撞属性
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//忽略所有通道
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);//无碰撞

	//创建碰撞球体组件
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);

	//创建粒子系统组件
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(RootComponent);

}

//游戏开始或生成时调用
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	//绑定回调函数到基本组件委托
	//重叠事件
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	//重叠结束事件
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

}

//计算正弦量
float AItem::SinFunction()
{
	return Amplitude * FMath::Sin(RunningTime * Frequency);
}

//计算余弦量
float AItem::CosFunction()
{
	return Amplitude * FMath::Cos(RunningTime * Frequency);
}

//重叠事件
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	//尝试将重叠物体转换为拾取接口
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		//调用接口函数，设置当前重叠的物品
		PickupInterface->SetOverlappingItem(this);
	}

	//const FString OtherActorName = OtherActor ? OtherActor->GetName() : FString("None");
	//if (GEngine)	//调试打印重叠物体名称
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, OtherActorName);
	//}
}

//重叠结束事件
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//尝试将重叠物体转换为拾取接口
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		//调用接口函数，清除当前重叠的物品
		PickupInterface->SetOverlappingItem(nullptr);
	}

	//const FString OtherActorName = OtherActor ? OtherActor->GetName() : FString("None");
	//if (GEngine)	//调试打印重叠物体名称
	//{
	//	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Blue, OtherActorName + FString(" End Overlap"));
	//}
}

//生成拾取特效
void AItem::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,		//世界上下文对象
			PickupEffect, //粒子系统
			GetActorLocation() //位置
		);
	}
}

//播放拾取音效
void AItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,		//世界上下文对象
			PickupSound, //音效
			GetActorLocation() //位置
		);
	}
}

//每一帧都调用
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//设置运行时间
	RunningTime += DeltaTime;

	//物品浮动
	if (ItemState == EItemState::EIS_Hovering)
	{
		//设置物体世界偏移量
		AddActorLocalOffset(FVector(0.f, 0.f, SinFunction()));
	}

	////移动速度
	//float MovementSpeed = 50.f;
	////旋转速度
	//float RotationSpeed = 30.f;
	////设置角色世界偏移量
	//AddActorWorldOffset(FVector(MovementSpeed * DeltaTime, 0.f, 0.f));
	////设置角色世界旋转量
	//AddActorWorldRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
	
	////计算正弦量
	//float DeltaZ = Amplitude * FMath::Sin(RunningTime * Frequency);
	////设置角色世界偏移量
	//AddActorLocalOffset(FVector(0.f, 0.f, DeltaZ));

	////绘制调试球
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	////绘制调试向量
	//DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation()+GetActorForwardVector()*100.f);

	////计算位置向量中点
	//FVector AvgVector = AVG<FVector>(GetActorLocation(), FVector::Zero);
	////绘制位置向量中点
	//DRAW_POINT_SingleFrame(AvgVector);
}


