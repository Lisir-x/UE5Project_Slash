
#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ABreakableActor::ABreakableActor()
{
	//����Ϊfalseʱ��ֹͣTick�¼�
 	PrimaryActorTick.bCanEverTick = false;

	//�������μ������������Ϊ�����
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	//���ü��μ����������ײ����
	GeometryCollection->SetGenerateOverlapEvents(true);//�����ص��¼�
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);//����Cameraͨ��
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);//����Pawnͨ��

	//�����������
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	//���ý����������ײ����
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//��������ͨ��
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);//�赲Pawnͨ��


}

//��Ϸ��ʼ������ʱ����
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

//ÿһ֡������
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//���к���
void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken) return;	//�����������ֱ�ӷ���
	bBroken = true;	//����Ϊ������
	//��ȡ�������
	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num() > 0)
	{
		////����������Ч
		//if (BreakSound)
		//{
		//	UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GetActorLocation());
		//}
		//���õ�����λ��
		FVector Location = GetActorLocation();
		Location.Z += 75.f;	//����������λ�ã���ֹ����ʱ������ص�
		//���������
		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		//���ɵ�����
		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
}

