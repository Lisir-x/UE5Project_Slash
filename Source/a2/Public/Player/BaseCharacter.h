// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "CharacterType.h"
#include "BaseCharacter.generated.h"

class ABaseWeapon;
class UAnimMontage;
class UAttributeComponent;

UCLASS()
class A2_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	//ÿһ֡������
	virtual void Tick(float DeltaTime) override;

protected:
	//��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;

	/*----------ս�����----------*/
	//���к������ܵ��˺�ʱ����
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	//����
	virtual void Attack();

	//�����߼�
	UFUNCTION(BlueprintNativeEvent)
	void Die();

	//�������ܻ���Ӧ
	void DirectionalHitReact(const FVector& ImpactPoint);
	//�����˺�
	virtual void HandleDamage(float DamageAmount);
	//�����ܻ���Ч
	void PlayHitSound(const FVector& ImpactPoint);
	//�����ܻ�����Ч��
	void SpawnHitParticles(const FVector& ImpactPoint);
	//���ý�������ײ
	void DisableCapsule();
	//�ж��Ƿ���Թ���
	virtual bool CanAttack();
	//�ж��Ƿ���
	bool IsAlive();
	//������������ײ
	void DisableMeshCollision();
	/*----------------------------*/

	/*----------��̫�����----------*/
	//�����ܻ���̫��
	void PlayHitReactMontage(const FName& SectionName);
	//���Ź�����̫��
	virtual int32 PlayAttackMontage();
	//����������̫��
	virtual int32 PlayDeathMontage();
	//�жϹ�����̫��
	void StopAttackMontage();
	//����������̫��
	virtual void PlayDodgeMontage();
	/*------------------------------*/

	//��ȡŤ��Ŀ��λ��
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	//��ȡŤ��Ŀ����ת
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	//��������
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	//���ܽ���
	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	//����������ײ����
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable);

	//���ù�����������ײ����
	UFUNCTION(BlueprintCallable)
	void SetMeshCollisionResponses(
		const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Responses);

	//��װ��������
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<ABaseWeapon> EquippedWeapon;

	//�������
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;

	//��ǰս��Ŀ��
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	//Ť��Ŀ�귶Χ
	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;

	//��������
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

private:

	//����ָ����̫���ָ����
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	//���������̫���
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	//�ܻ���Ч
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USoundBase> HitSound;

	//����ϵͳ
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UParticleSystem> HitParticles;

	/*----------������̫��----------*/
	//������̫��
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

	//�ܻ���̫��
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	//������̫��
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> DeathMontage;

	//������̫��
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> DodgeMontage;

	//������̫�������
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackMontageSections;

	//������̫�������
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> DeathMontageSections;
	/*------------------------------*/

public:
	//��ȡ��������
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
};
