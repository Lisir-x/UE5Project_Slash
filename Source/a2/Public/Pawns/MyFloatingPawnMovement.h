#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyFloatingPawnMovement.generated.h"

UCLASS()
class A2_API UMyFloatingPawnMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()
	
public:
	UMyFloatingPawnMovement();

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//��ת���ʣ�ÿ�������ת�Ƕȣ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator RotationRate = FRotator(180.f, 500.f, 500.f); //Pitch, Yaw, Roll

	//�Ƿ�ʹ�ÿ���������ת
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bUseControllerDesiredRotation = false;

	//�Ƿ���ת�����˶�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bOrientRotationToMovement = true;

	//�Ƿ�����Pitch��ת
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bAllowPitchRotation = true;
};
