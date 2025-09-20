
#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "BaseWeapon.generated.h"

class USoundBase;
class UBoxComponent;

UCLASS()
class A2_API ABaseWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	ABaseWeapon();

	//装备武器
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigetor);
	//禁用碰撞球体
	void DisableSphereCollision();
	//播放装备音效
	void PlayEquipSound();
	//禁用粒子系统组件
	void DeactivateEmbers();

	//将武器网格附加到指定组件的指定插槽
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	//需要忽略的碰撞体
	TArray<AActor*> IgnoreActors;

protected:
	//游戏开始或生成时调用
	virtual void BeginPlay() override;

	//武器碰撞盒重叠事件
	UFUNCTION()
	void OnBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	//判断是否为相同类型的Actor
	bool ActorIsSameType(AActor* OtherActor);

	//执行命中检测
	void ExecuteGetHit(FHitResult& BoxHit);

	//创建力场
	UFUNCTION(BlueprintImplementableEvent)	//蓝图实现事件
	void CreateFields(const FVector& FieldLocation);

private:
	//盒形追踪
	void BoxTrace(FHitResult& BoxHit);

	//盒形追踪范围
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FVector BoxTraceExtent = FVector(5.f);

	//是否显示盒形追踪调试线
	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bShowBoxDebug = false;

	//装备音效
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<USoundBase> EquipSound;

	//碰撞盒组件
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<UBoxComponent> WeaponBox;

	//盒形追踪起点组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceStart;

	//盒形追踪终点组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceEnd;

	//武器伤害
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float Damage = 20.f;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
