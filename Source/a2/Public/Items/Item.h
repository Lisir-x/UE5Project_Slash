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
	//ÿһ֡������
	virtual void Tick(float DeltaTime) override;

protected:
	//��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;

	//���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amplitude = 0.2f;
	//Ƶ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Frequency = 5.f;
	//����������
	UFUNCTION(BlueprintPure)
	float SinFunction();
	//����������
	UFUNCTION(BlueprintPure)
	float CosFunction();

	//ģ�庯��
	template<typename T>
	T AVG(T a, T b);

	/*-----�󶨵��������ί�еĻص�����-----*/
	//�ص��¼�
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	//�ص������¼�
	UFUNCTION()
	virtual void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	/*--------------------------------------*/

	//����ʰȡ��Ч
	virtual void SpawnPickupSystem();

	//����ʰȡ��Ч
	virtual void SpawnPickupSound();

	//��̬���������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	//��ײ�������
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	//��Ʒ״̬
	EItemState ItemState = EItemState::EIS_Hovering;

	//Nigara����ϵͳ���
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> ItemEffect;

	//ʰȡ��Ч
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> PickupSound;


private:
	//����ʱ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float RunningTime;

	//ʰȡ��Ч
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> PickupEffect;
};

template<typename T>
inline T AItem::AVG(T a, T b)
{
	return (a + b) / 2;
}
