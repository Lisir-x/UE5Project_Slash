#include "Pawns/MyFloatingPawnMovement.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"


UMyFloatingPawnMovement::UMyFloatingPawnMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMyFloatingPawnMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent) return;

	FRotator CurrentRotation = PawnOwner->GetActorRotation();
	FRotator TargetRotation = CurrentRotation;

	//��������ת
	if (bUseControllerDesiredRotation && PawnOwner->GetController())
	{
		TargetRotation = PawnOwner->GetController()->GetControlRotation();

		if (!bAllowPitchRotation) TargetRotation.Pitch = 0.f;
		TargetRotation.Roll = 0.f;
	}
	//�˶�������ת
	else if (bOrientRotationToMovement && !Velocity.IsNearlyZero())
	{
		TargetRotation = Velocity.GetSafeNormal().Rotation();

		if (!bAllowPitchRotation) TargetRotation.Pitch = 0.f;
		TargetRotation.Roll = 0.f;
	}

	//��ֵ��ת
	if (!TargetRotation.Equals(CurrentRotation))
	{
		FRotator NewRotation;
		NewRotation.Pitch = FMath::FInterpConstantTo(CurrentRotation.Pitch, TargetRotation.Pitch, DeltaTime, RotationRate.Pitch);
		NewRotation.Yaw = FMath::FInterpConstantTo(CurrentRotation.Yaw, TargetRotation.Yaw, DeltaTime, RotationRate.Yaw);
		NewRotation.Roll = FMath::FInterpConstantTo(CurrentRotation.Roll, TargetRotation.Roll, DeltaTime, RotationRate.Roll);

		PawnOwner->SetActorRotation(NewRotation);
	}
}

