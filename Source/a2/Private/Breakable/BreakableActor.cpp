
#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ABreakableActor::ABreakableActor()
{
	//当设为false时将停止Tick事件
 	PrimaryActorTick.bCanEverTick = false;

	//创建几何集合组件并设置为根组件
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	//设置几何集合组件的碰撞属性
	GeometryCollection->SetGenerateOverlapEvents(true);//生成重叠事件
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);//忽略Camera通道
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);//忽略Pawn通道

	//创建胶囊组件
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	//设置胶囊组件的碰撞属性
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//忽略所有通道
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);//阻挡Pawn通道


}

//游戏开始或生成时调用
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

//每一帧都调用
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//命中函数
void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken) return;	//如果已破裂则直接返回
	bBroken = true;	//设置为已破裂
	//获取世界对象
	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num() > 0)
	{
		////播放碎裂音效
		//if (BreakSound)
		//{
		//	UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GetActorLocation());
		//}
		//设置掉落物位置
		FVector Location = GetActorLocation();
		Location.Z += 75.f;	//提升掉落物位置，防止生成时与地面重叠
		//生成随机数
		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		//生成掉落物
		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
}

