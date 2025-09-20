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
 	//当设为false时将停止Tick事件
	PrimaryActorTick.bCanEverTick = true;

	//创建胶囊默认子对象
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	//Capsule->SetupAttachment(RootComponent);
	SetRootComponent(Capsule);
	//设置胶囊半高和半径
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);

	//创建骨骼网格体默认子对象
	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(Capsule);//RootComponent,GetRoomComponent()效果一样

	//创建弹簧臂默认子对象
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(Capsule);
	//弹簧臂相关参数设置
	CameraBoom->TargetArmLength = 300.0f;//臂长
	CameraBoom->bUsePawnControlRotation = true; //允许弹簧臂跟随控制器旋转

	//创建摄像机默认子对象
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//创建角色移动组件默认子对象
	PawnMovement = CreateDefaultSubobject<UMyFloatingPawnMovement>(TEXT("PawnMovement"));

	//角色不跟随控制器旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//角色朝向移动方向
	PawnMovement->bOrientRotationToMovement = true;
	//每秒旋转500度
	PawnMovement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

}

//游戏开始或生成时调用
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	// 获取当前角色的控制器，并尝试将其转换为玩家控制器
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// 通过玩家控制器获取本地玩家，并获取增强输入子系统
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 将输入映射上下文添加到子系统中，优先级为0
			Subsystem->AddMappingContext(IMC_Bird, 0);
		}
	}

}

void ABird::BirdMove(const FInputActionValue& Value)
{
	//获取输入的三维向量
	FVector MoveVector = Value.Get<FVector>();
	if (Controller)	//如果有控制器
	{
		//获取控制器的旋转
		const FRotator ControlRotation = Controller->GetControlRotation();
		//计算三维方向向量
		const FVector Forward = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);//前后
		const FVector Right = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);//左右
		const FVector Up = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Z);//升降
		//应用三维向量
		AddMovementInput(Forward, MoveVector.Y);
		AddMovementInput(Right, MoveVector.X);
		AddMovementInput(Up, MoveVector.Z);
	}
}

void ABird::BirdLook(const FInputActionValue& Value)
{
	//获取输入的二维向量
	FVector2D LookVector = Value.Get<FVector2D>();
	if (Controller)	//如果有控制器
	{
		//添加控制器的偏航(X)和俯仰(Y)输入
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(-LookVector.Y);
	}
}

//每一帧都调用
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//用于绑定输入
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//绑定移动
		EnhancedInputComponent->BindAction(IA_BirdMove,
			ETriggerEvent::Triggered, this, &ABird::BirdMove);
		//绑定视角
		EnhancedInputComponent->BindAction(IA_BirdLook,
			ETriggerEvent::Triggered, this, &ABird::BirdLook);
	}

}

