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
 	//����Ϊfalseʱ��ֹͣTick�¼�
	PrimaryActorTick.bCanEverTick = true;

	//�����������
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//������̬���������
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	//���þ�̬��������ײ����
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//��������ͨ��
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);//����ײ

	//������ײ�������
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);

	//��������ϵͳ���
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(RootComponent);

}

//��Ϸ��ʼ������ʱ����
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	//�󶨻ص��������������ί��
	//�ص��¼�
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	//�ص������¼�
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

}

//����������
float AItem::SinFunction()
{
	return Amplitude * FMath::Sin(RunningTime * Frequency);
}

//����������
float AItem::CosFunction()
{
	return Amplitude * FMath::Cos(RunningTime * Frequency);
}

//�ص��¼�
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	//���Խ��ص�����ת��Ϊʰȡ�ӿ�
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		//���ýӿں��������õ�ǰ�ص�����Ʒ
		PickupInterface->SetOverlappingItem(this);
	}

	//const FString OtherActorName = OtherActor ? OtherActor->GetName() : FString("None");
	//if (GEngine)	//���Դ�ӡ�ص���������
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, OtherActorName);
	//}
}

//�ص������¼�
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//���Խ��ص�����ת��Ϊʰȡ�ӿ�
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		//���ýӿں����������ǰ�ص�����Ʒ
		PickupInterface->SetOverlappingItem(nullptr);
	}

	//const FString OtherActorName = OtherActor ? OtherActor->GetName() : FString("None");
	//if (GEngine)	//���Դ�ӡ�ص���������
	//{
	//	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Blue, OtherActorName + FString(" End Overlap"));
	//}
}

//����ʰȡ��Ч
void AItem::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,		//���������Ķ���
			PickupEffect, //����ϵͳ
			GetActorLocation() //λ��
		);
	}
}

//����ʰȡ��Ч
void AItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,		//���������Ķ���
			PickupSound, //��Ч
			GetActorLocation() //λ��
		);
	}
}

//ÿһ֡������
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//��������ʱ��
	RunningTime += DeltaTime;

	//��Ʒ����
	if (ItemState == EItemState::EIS_Hovering)
	{
		//������������ƫ����
		AddActorLocalOffset(FVector(0.f, 0.f, SinFunction()));
	}

	////�ƶ��ٶ�
	//float MovementSpeed = 50.f;
	////��ת�ٶ�
	//float RotationSpeed = 30.f;
	////���ý�ɫ����ƫ����
	//AddActorWorldOffset(FVector(MovementSpeed * DeltaTime, 0.f, 0.f));
	////���ý�ɫ������ת��
	//AddActorWorldRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
	
	////����������
	//float DeltaZ = Amplitude * FMath::Sin(RunningTime * Frequency);
	////���ý�ɫ����ƫ����
	//AddActorLocalOffset(FVector(0.f, 0.f, DeltaZ));

	////���Ƶ�����
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	////���Ƶ�������
	//DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation()+GetActorForwardVector()*100.f);

	////����λ�������е�
	//FVector AvgVector = AVG<FVector>(GetActorLocation(), FVector::Zero);
	////����λ�������е�
	//DRAW_POINT_SingleFrame(AvgVector);
}

