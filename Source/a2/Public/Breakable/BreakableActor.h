#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class ATreasure;
class UCapsuleComponent;

UCLASS()
class A2_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();

	//每一帧都调用
	virtual void Tick(float DeltaTime) override;

	//命中函数
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	//游戏开始或生成时调用
	virtual void BeginPlay() override;

	//几何集合组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

	//胶囊组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> Capsule;

private:
	////破裂音效
	//UPROPERTY(EditAnywhere, Category = "Sound")
	//TObjectPtr<USoundBase> BreakSound;

	//宝藏类(掉落物)
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ATreasure>> TreasureClasses;

	//是否已破裂
	bool bBroken = false;

};
