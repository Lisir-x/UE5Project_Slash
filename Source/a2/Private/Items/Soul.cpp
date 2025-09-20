// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/KismetSystemLibrary.h"

//ÿһ֡������
void ASoul::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	//�����ٶȺ�ʱ�������һ֡λ��
	const double LocationZ = GetActorLocation().Z; //��ȡ��ǰλ�õ�Z����
	if (LocationZ > DesiredZ) //�����ǰλ�õ�Z�������Ŀ��λ�õ�Z����
	{
		//�����ƶ�
		const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
		AddActorWorldOffset(DeltaLocation);
	}
}

//��Ϸ��ʼ������ʱ����
void ASoul::BeginPlay()
{
	Super::BeginPlay();

	//��ȡĿ��λ�úͽ���λ����Ϊ���ߵ������յ�
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);

	//�����Ҫ������������
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	//�����Ҫ���Ե�����
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	//���ڴ洢���߼����
	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this, Start, End,	//������㡢�յ�
		ObjectTypes,		//��Ҫ������������
		false,				//��ʹ�ø�����ײ
		ActorsToIgnore,		//��Ҫ���Ե�Actor����
		EDrawDebugTrace::None, //���Ի���ģʽ
		HitResult,			//�洢���߼����
		true				//�Ƿ��������
	);
	//��ȡ���߼������λ��
	DesiredZ = HitResult.ImpactPoint.Z + 50.f;
}

//�ص��¼�
void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���ص�����ת��Ϊʰȡ�ӿ�
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		//���ýӿں�����������
		PickupInterface->AddSouls(this);
		//����ʰȡ��Ч
		SpawnPickupSystem();
		//����ʰȡ��Ч
		SpawnPickupSound();
		//�������
		Destroy();
	}

}
