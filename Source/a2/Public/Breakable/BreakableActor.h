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

	//ÿһ֡������
	virtual void Tick(float DeltaTime) override;

	//���к���
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	//��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;

	//���μ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

	//�������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> Capsule;

private:
	////������Ч
	//UPROPERTY(EditAnywhere, Category = "Sound")
	//TObjectPtr<USoundBase> BreakSound;

	//������(������)
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ATreasure>> TreasureClasses;

	//�Ƿ�������
	bool bBroken = false;

};
