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
	//旋转速率（每秒最大旋转角度）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator RotationRate = FRotator(180.f, 500.f, 500.f); //Pitch, Yaw, Roll

	//是否使用控制器的旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bUseControllerDesiredRotation = false;

	//是否将旋转朝向运动方向
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bOrientRotationToMovement = true;

	//是否允许Pitch旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bAllowPitchRotation = true;
};
