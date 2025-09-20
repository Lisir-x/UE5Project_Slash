// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AttributeComponent.h"


UAttributeComponent::UAttributeComponent()
{
	//����Ϊfalseʱ��ֹͣTick�¼�
	PrimaryComponentTick.bCanEverTick = false;
	
}

//��Ϸ��ʼ������ʱ����
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}

//�ܵ��˺�
void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

//��������
void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

//��ȡ�����ٷֱ�
float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

//��ȡ�����ٷֱ�
float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

//������
void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
	Souls += NumberOfSouls;
}

//��ӻƽ�
void UAttributeComponent::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}

//ÿһ֡������
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

//�����ָ�
void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

