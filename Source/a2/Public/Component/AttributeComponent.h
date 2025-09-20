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

	//ÿһ֡������
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//�����ָ�
	void RegenStamina(float DeltaTime);

protected:
	//��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;

private:
	//��ǰ����ֵ
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float Health;
	
	//�������ֵ
	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float MaxHealth;

	//��ǰ����ֵ
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;

	//�������ֵ
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	//�ƽ�����
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	//�������
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	//�������ĵ�����ֵ
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost;

	//����ֵ�ָ�����
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate;

public:
	//�ܵ��˺�
	void ReceiveDamage(float Damage);

	//��������
	void UseStamina(float StaminaCost);

	//��ȡ�����ٷֱ�
	float GetHealthPercent();

	//��ȡ�����ٷֱ�
	float GetStaminaPercent();

	//�ж��Ƿ���
	bool IsAlive() const { return Health > 0.f; }

	//������
	void AddSouls(int32 NumberOfSouls);

	//��ӻƽ�
	void AddGold(int32 AmountOfGold);

	//��ȡ�ƽ�����
	FORCEINLINE int32 GetGold() const { return Gold; }

	//��ȡ�������
	FORCEINLINE int32 GetSouls() const { return Souls; }

	//��ȡ�������ĵ�����ֵ
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }

	//��ȡ��ǰ����ֵ
	FORCEINLINE float GetStamina() const { return Stamina; }
};
