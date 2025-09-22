// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AttributeComponent.h"


UAttributeComponent::UAttributeComponent()
{
	//当设为false时将停止Tick事件
	PrimaryComponentTick.bCanEverTick = false;
	
}

//游戏开始或生成时调用
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}

//受到伤害
void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

//消耗耐力
void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

//获取生命百分比
float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

//获取耐力百分比
float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

//添加灵魂
void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
	Souls += NumberOfSouls;
}

//添加黄金
void UAttributeComponent::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}

//每一帧都调用
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

//耐力恢复
void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}


