// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class A2_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	//每一帧都调用
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//耐力恢复
	void RegenStamina(float DeltaTime);

protected:
	//游戏开始或生成时调用
	virtual void BeginPlay() override;

private:
	//当前生命值
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float Health;
	
	//最大生命值
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float MaxHealth;

	//当前耐力值
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;

	//最大耐力值
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	//黄金数量
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	//灵魂数量
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	//闪避消耗的耐力值
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost;

	//耐力值恢复速率
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate;

public:
	//受到伤害
	void ReceiveDamage(float Damage);

	//消耗耐力
	void UseStamina(float StaminaCost);

	//获取生命百分比
	float GetHealthPercent();

	//获取耐力百分比
	float GetStaminaPercent();

	//判断是否存活
	bool IsAlive() const { return Health > 0.f; }

	//添加灵魂
	void AddSouls(int32 NumberOfSouls);

	//添加黄金
	void AddGold(int32 AmountOfGold);

	//获取黄金数量
	FORCEINLINE int32 GetGold() const { return Gold; }

	//获取灵魂数量
	FORCEINLINE int32 GetSouls() const { return Souls; }

	//获取闪避消耗的耐力值
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }

	//获取当前耐力值
	FORCEINLINE float GetStamina() const { return Stamina; }
};

