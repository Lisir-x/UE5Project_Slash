

#include "Weapon/BaseWeapon.h"
#include "Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"


ABaseWeapon::ABaseWeapon()
{
	//������ײ�����Ĭ���Ӷ���
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(ItemMesh);
	//������ײ���������ײ����
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);//����ײ
	WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);//���綯̬����
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);//������ͨ���ص�
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);//����Pawnͨ��

	//����BoxTraceStart���Ĭ���Ӷ���
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(ItemMesh);

	//����BoxTraceEnd���Ĭ���Ӷ���
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(ItemMesh);
}

//װ������
void ABaseWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigetor)
{

	//������Ʒ״̬Ϊװ��
	ItemState = EItemState::EIS_Equipped;
	//����������ӵ���ߺ�ִ����
	SetOwner(NewOwner);
	SetInstigator(NewInstigetor);
	//���������񸽼ӵ�ָ�������ָ�����
	AttachMeshToSocket(InParent, InSocketName);
	//������ײ����
	DisableSphereCollision();
	//����װ����Ч
	PlayEquipSound();
	//��������ϵͳ���
	DeactivateEmbers();
}

//������ײ����
void ABaseWeapon::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//����װ����Ч
void ABaseWeapon::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
}

//��������ϵͳ���
void ABaseWeapon::DeactivateEmbers()
{
	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

//���������񸽼ӵ�ָ�������ָ�����
void ABaseWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	//���帽�ӹ���
	//����1�������������λ�ú���ת����Ϊ��۵�λ�ú���ת�����ű��ֲ���
	//����2���Ƿ񱣳����λ�ã����������壩
	FAttachmentTransformRules TransformRule(EAttachmentRule::SnapToTarget, true);
	//���������񸽼ӵ���ҹ���������
	//FAttachmentTransformRules::SnapToTargetIncludingScale�滻TransformRule��
	//�����������λ�á���ת�����Ŷ�����Ϊ��۵�λ�á���ת�����ţ�������������
	ItemMesh->AttachToComponent(InParent, TransformRule, InSocketName);
}

//��Ϸ��ʼ������ʱ����
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	//�󶨻ص��������������ί��
	//������ײ���ص��¼�
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnBoxOverlap);
}

//������ײ���ص��¼�
void ABaseWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	//����ص���Actor��ͬ�࣬��ֱ�ӷ���
	if (ActorIsSameType(OtherActor)) return;

	//�洢��ײ������Ľṹ��
	FHitResult BoxHit;
	//����׷��
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())	//�������ײ
	{
		//�����ײ����Actor��ͬ�࣬��ֱ�ӷ���
		if (ActorIsSameType(BoxHit.GetActor())) return;
		//Ӧ���˺�
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),	//�ܻ�Actor
			Damage,				//�˺�ֵ
			GetInstigator()->GetController(),//����˺��Ŀ�����
			this,				//����˺���Actor
			UDamageType::StaticClass()	//�˺�����
		);
		//ִ�����м��
		ExecuteGetHit(BoxHit);
		//���ô�����������
		CreateFields(BoxHit.ImpactPoint);

	}
}

//�ж��Ƿ�Ϊ��ͬ���͵�Actor
bool ABaseWeapon::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

//ִ�����м��
void ABaseWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	//���Խ���ײ����Actorת��Ϊ���нӿڣ��ж�Ŀ���Ƿ�ʵ�����ܻ��߼���
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)	//���ʵ�����ܻ��ӿ�
	{
		//�����ܻ��ӿڵ��ܻ�����
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

//����׷��
void ABaseWeapon::BoxTrace(FHitResult& BoxHit)
{
	//��ȡ׷�����λ��
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	//������Ҫ���Ե�Actor���飨����������
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);//����ǰ���������������б�
	ActorsToIgnore.Add(GetOwner());//��������ӵ���߼�������б�
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);//�������б��е�Actor��������б�
	}

	//ִ�к�����ײ���
	UKismetSystemLibrary::BoxTraceSingle(
		this, Start, End,	//������㡢�յ�
		BoxTraceExtent,		//�����ߴ磨��ά������
		BoxTraceStart->GetComponentRotation(), //ʹ��ָ���������ת����
		ETraceTypeQuery::TraceTypeQuery1, //ʹ�õ���ײͨ������
		false,	//����⸴����ײ����Ϊtrue�ɼ�������μ������ײ��
		ActorsToIgnore, //��Ҫ���Ե�Actor����
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,//���Ի���ģʽ
		BoxHit,	//���������������ײ���
		true	//�Ƿ����������ActorToIgnore���ʹ�ã�
	);
	//����ײ����Actor��������б�
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
