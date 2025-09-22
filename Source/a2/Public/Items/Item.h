// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped,
};

UCLASS()
class A2_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	//每一帧都调用
	virtual void Tick(float DeltaTime) override;

protected:
	//游戏开始或生成时调用
	virtual void BeginPlay() override;

	//振幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amplitude = 0.2f;
	//频率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Frequency = 5.f;
	//计算正弦量
	UFUNCTION(BlueprintPure)
	float SinFunction();
	//计算余弦量
	UFUNCTION(BlueprintPure)
	float CosFunction();

	//模板函数
	template<typename T>
	T AVG(T a, T b);

	/*-----绑定到基本组件委托的回调函数-----*/
	//重叠事件
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	//重叠结束事件
	UFUNCTION()
	virtual void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	/*--------------------------------------*/

	//生成拾取特效
	virtual void SpawnPickupSystem();

	//播放拾取音效
	virtual void SpawnPickupSound();

	//静态网格体组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	//碰撞球体组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	//物品状态
	EItemState ItemState = EItemState::EIS_Hovering;

	//Nigara粒子系统组件
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> ItemEffect;

	//拾取音效
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> PickupSound;


private:
	//运行时间
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float RunningTime;

	//拾取特效
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> PickupEffect;
};

template<typename T>
inline T AItem::AVG(T a, T b)
{
	return (a + b) / 2;
}

