// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/KismetSystemLibrary.h"

//每一帧都调用
void ASoul::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	//根据速度和时间计算下一帧位置
	const double LocationZ = GetActorLocation().Z; //获取当前位置的Z坐标
	if (LocationZ > DesiredZ) //如果当前位置的Z坐标大于目标位置的Z坐标
	{
		//向下移动
		const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
		AddActorWorldOffset(DeltaLocation);
	}
}

//游戏开始或生成时调用
void ASoul::BeginPlay()
{
	Super::BeginPlay();

	//获取目标位置和结束位置作为射线的起点和终点
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);

	//添加需要检测的物体类型
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	//添加需要忽略的物体
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	//用于存储射线检测结果
	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this, Start, End,	//对象、起点、终点
		ObjectTypes,		//需要检测的物体类型
		false,				//不使用复杂碰撞
		ActorsToIgnore,		//需要忽略的Actor数组
		EDrawDebugTrace::None, //调试绘制模式
		HitResult,			//存储射线检测结果
		true				//是否忽略自身
	);
	//获取射线检测结果的位置
	DesiredZ = HitResult.ImpactPoint.Z + 50.f;
}

//重叠事件
void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//将重叠物体转换为拾取接口
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		//调用接口函数，添加灵魂
		PickupInterface->AddSouls(this);
		//生成拾取特效
		SpawnPickupSystem();
		//播放拾取音效
		SpawnPickupSound();
		//销毁灵魂
		Destroy();
	}

}
