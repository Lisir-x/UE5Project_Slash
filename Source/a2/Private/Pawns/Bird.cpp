// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Pawns/MyFloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ABird::ABird()
{
 	//����Ϊfalseʱ��ֹͣTick�¼�
	PrimaryActorTick.bCanEverTick = true;

	//��������Ĭ���Ӷ���
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	//Capsule->SetupAttachment(RootComponent);
	SetRootComponent(Capsule);
	//���ý��Ұ�ߺͰ뾶
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);

	//��������������Ĭ���Ӷ���
	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(Capsule);//RootComponent,GetRoomComponent()Ч��һ��

	//�������ɱ�Ĭ���Ӷ���
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(Capsule);
	//���ɱ���ز�������
	CameraBoom->TargetArmLength = 300.0f;//�۳�
	CameraBoom->bUsePawnControlRotation = true; //�����ɱ۸����������ת

	//���������Ĭ���Ӷ���
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//������ɫ�ƶ����Ĭ���Ӷ���
	PawnMovement = CreateDefaultSubobject<UMyFloatingPawnMovement>(TEXT("PawnMovement"));

	//��ɫ�������������ת
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//��ɫ�����ƶ�����
	PawnMovement->bOrientRotationToMovement = true;
	//ÿ����ת500��
	PawnMovement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

}

//��Ϸ��ʼ������ʱ����
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	// ��ȡ��ǰ��ɫ�Ŀ������������Խ���ת��Ϊ��ҿ�����
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// ͨ����ҿ�������ȡ������ң�����ȡ��ǿ������ϵͳ
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// ������ӳ����������ӵ���ϵͳ�У����ȼ�Ϊ0
			Subsystem->AddMappingContext(IMC_Bird, 0);
		}
	}

}

void ABird::BirdMove(const FInputActionValue& Value)
{
	//��ȡ�������ά����
	FVector MoveVector = Value.Get<FVector>();
	if (Controller)	//����п�����
	{
		//��ȡ����������ת
		const FRotator ControlRotation = Controller->GetControlRotation();
		//������ά��������
		const FVector Forward = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);//ǰ��
		const FVector Right = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);//����
		const FVector Up = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Z);//����
		//Ӧ����ά����
		AddMovementInput(Forward, MoveVector.Y);
		AddMovementInput(Right, MoveVector.X);
		AddMovementInput(Up, MoveVector.Z);
	}
}

void ABird::BirdLook(const FInputActionValue& Value)
{
	//��ȡ����Ķ�ά����
	FVector2D LookVector = Value.Get<FVector2D>();
	if (Controller)	//����п�����
	{
		//��ӿ�������ƫ��(X)�͸���(Y)����
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(-LookVector.Y);
	}
}

//ÿһ֡������
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//���ڰ�����
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//���ƶ�
		EnhancedInputComponent->BindAction(IA_BirdMove,
			ETriggerEvent::Triggered, this, &ABird::BirdMove);
		//���ӽ�
		EnhancedInputComponent->BindAction(IA_BirdLook,
			ETriggerEvent::Triggered, this, &ABird::BirdLook);
	}

}

