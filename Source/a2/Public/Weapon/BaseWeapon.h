
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

	//װ������
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigetor);
	//������ײ����
	void DisableSphereCollision();
	//����װ����Ч
	void PlayEquipSound();
	//��������ϵͳ���
	void DeactivateEmbers();

	//���������񸽼ӵ�ָ�������ָ�����
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	//��Ҫ���Ե���ײ��
	TArray<AActor*> IgnoreActors;

protected:
	//��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;

	//������ײ���ص��¼�
	UFUNCTION()
	void OnBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	//�ж��Ƿ�Ϊ��ͬ���͵�Actor
	bool ActorIsSameType(AActor* OtherActor);

	//ִ�����м��
	void ExecuteGetHit(FHitResult& BoxHit);

	//��������
	UFUNCTION(BlueprintImplementableEvent)	//��ͼʵ���¼�
	void CreateFields(const FVector& FieldLocation);

private:
	//����׷��
	void BoxTrace(FHitResult& BoxHit);

	//����׷�ٷ�Χ
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FVector BoxTraceExtent = FVector(5.f);

	//�Ƿ���ʾ����׷�ٵ�����
	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bShowBoxDebug = false;

	//װ����Ч
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<USoundBase> EquipSound;

	//��ײ�����
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<UBoxComponent> WeaponBox;

	//����׷��������
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceStart;

	//����׷���յ����
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceEnd;

	//�����˺�
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float Damage = 20.f;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
