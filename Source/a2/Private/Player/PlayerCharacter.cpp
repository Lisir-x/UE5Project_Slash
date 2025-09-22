

#include "Player/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/AttributeComponent.h"
#include "GameFramework/Pawn.h"
#include "Items/Item.h"
#include "Weapon/BaseWeapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

APlayerCharacter::APlayerCharacter()
{
 	//当设为false时将停止Tick事件
	PrimaryActorTick.bCanEverTick = true;

	//创建弹簧臂组件
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	//设置弹簧臂组件属性
	CameraBoom->bUsePawnControlRotation = true; //允许弹簧臂跟随控制器旋转
	CameraBoom->TargetArmLength = 300.0f;	//弹簧臂长度
	CameraBoom->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f)); //弹簧臂相对旋转

	//创建摄像机组件
	Camera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//设置摄像机组件属性
	//角色不跟随控制器旋转
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//设置角色移动组件属性
	GetCharacterMovement()->bOrientRotationToMovement = true; //角色朝向移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); //每秒旋转速率
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;	//角色最大行走速度
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f; //角色最小模拟行走速度

	//设置网格体碰撞属性
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);	//将网格体设置为动态物体
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //忽略所有碰撞
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);//阻挡可见性通道
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);//重叠动态物体通道
	GetMesh()->SetGenerateOverlapEvents(true);	//可生成重叠事件

}

//每一帧都调用
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Attributes && PlayerOverlay)
	{
		//调用耐力恢复函数
		Attributes->RegenStamina(DeltaTime);
		//设置UI体力百分比
		PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

//用于绑定输入
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//绑定移动
		EnhancedInputComponent->BindAction(IA_Move,
			ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		//绑定视角
		EnhancedInputComponent->BindAction(IA_Look,
			ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		//绑定跳跃
		EnhancedInputComponent->BindAction(IA_Jump,
			ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		//绑定拾取或装备
		EnhancedInputComponent->BindAction(IA_Equip,
			ETriggerEvent::Started, this, &APlayerCharacter::FKeyPressed);
		//绑定攻击
		EnhancedInputComponent->BindAction(IA_Attack,
			ETriggerEvent::Started, this, &APlayerCharacter::Attack);
		//绑定闪避
		EnhancedInputComponent->BindAction(IA_Dodge,
			ETriggerEvent::Started, this, &APlayerCharacter::Dodge);

	}
}

//跳跃函数
void APlayerCharacter::Jump()
{
	//只有在空闲状态下才能跳跃
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

//受伤函数
float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	//调用处理伤害函数
	HandleDamage(DamageAmount);
	//更新UI血量
	SetHUDHealth();

	return DamageAmount;
}

//命中函数，受到伤害时调用
void APlayerCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	//禁用武器碰撞
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	//若属性组件存在且血量大于0
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		//设置当前动作状态为受击反应
		ActionState = EActionState::EAS_HitReaction;
	}

}

//设置当前重叠的物品
void APlayerCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

//添加灵魂
void APlayerCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && PlayerOverlay)
	{
		//添加灵魂到属性组件
		Attributes->AddSouls(Soul->GetSouls());
		//设置UI灵魂数量
		PlayerOverlay->SetSouls(Attributes->GetSouls());
	}
}

//添加黄金
void APlayerCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && PlayerOverlay)
	{
		//添加黄金到属性组件
		Attributes->AddGold(Treasure->GetGold());
		//设置UI黄金数量
		PlayerOverlay->SetGold(Attributes->GetGold());
	}
}

//游戏开始或生成时调用
void APlayerCharacter::BeginPlay()
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
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	//为角色添加标签
	Tags.Add(FName("EngageableTarget"));
	//初始化玩家UI
	InitializePlayerOverlay();

}

//移动输入处理
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	//获取输入的二维向量
	FVector2D MoveVector = Value.Get<FVector2D>();
	//如果处于非空闲状态则不移动
	if (!IsUnoccupied()) return;
	if (Controller)	//如果有控制器
	{
		//获取控制器的旋转
		const FRotator ControlRotation = Controller->GetControlRotation();
		//只获取偏航角(Yaw)
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		//根据偏航角获取前方向(Y)和右方向(X)
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//根据输入向量的X和Y分量分别沿前方向(Y)和右方向(X)移动角色
		AddMovementInput(ForwardDirection, MoveVector.Y);
		AddMovementInput(RightDirection, MoveVector.X);
		//中断攻击动画
		StopAttackMontage();
	}
	//FVector InputVector = FVector(MoveVector, 0.0f);
	//GetPawn()->AddMovementInput(InputVector);
}

//视角输入处理
void APlayerCharacter::Look(const FInputActionValue& Value)
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

//F键输入处理
void APlayerCharacter::FKeyPressed()
{
	//尝试将当前可拾取的物品转换为武器
	ABaseWeapon* OverlappingWeapon = Cast<ABaseWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			//如果有装备武器，则销毁
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon); //装备武器
	}
	else //若没有可拾取的物品，则执行卸装或装备操作
	{
		if (CanDisarm())
		{
			Disarm(); //卸装
		}
		else if (CanArm())
		{
			Arm(); //装备
		}
	}
}

//攻击
void APlayerCharacter::Attack()
{
	Super::Attack();

	//若当前动作状态为空闲，则播放攻击动画
	if (CanAttack())
	{
		PlayAttackMontage();
		//设置当前动作状态为攻击中
		ActionState = EActionState::EAS_Attacking;
	}
}

//闪避
void APlayerCharacter::Dodge()
{
	//若当前动作状态不为空闲或体力不足，则不执行闪避
	if (!IsUnoccupied() || !HasEnoughStamina()) return;
	//播放闪避动画
	PlayDodgeMontage();
	//设置当前动作状态为闪避
	ActionState = EActionState::EAS_Dodge;

	if (Attributes && PlayerOverlay)
	{
		//调用消耗体力函数
		Attributes->UseStamina(Attributes->GetDodgeCost());
		//设置UI体力百分比
		PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

//装备武器
void APlayerCharacter::EquipWeapon(ABaseWeapon* Weapon)
{
	//若有可拾取的武器，则装备该武器
	Weapon->Equip(GetMesh(), FName("RHSocket"), this, this);
	//角色状态设为单手装备
	CharacterState = ECharacterState::ECS_EquippedOneHanded;
	//将当前可拾取的武器设为已装备武器
	EquippedWeapon = Weapon;
}

//攻击结束
void APlayerCharacter::AttackEnd()
{
	//攻击结束后将动作状态设置为空闲
	ActionState = EActionState::EAS_Unoccupied;
}

//闪避结束
void APlayerCharacter::DodgeEnd()
{
	//闪避结束后将动作状态设置为空闲
	ActionState = EActionState::EAS_Unoccupied;
}

//判断是否可以攻击
bool APlayerCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

//判断是否可以卸装
bool APlayerCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

//判断是否可以装备
bool APlayerCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped && 
		EquippedWeapon;
}

//判断是否有足够体力
bool APlayerCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

//判断当前是否空闲状态
bool APlayerCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

//卸装
void APlayerCharacter::Disarm()
{
	//如果当前状态为可卸装，则卸装，播放卸装动画
	PlayEquipMontage(FName("Unequip"));
	//卸装后角色状态设为未装备
	CharacterState = ECharacterState::ECS_Unequipped;
	//设置当前动作状态为装备中
	ActionState = EActionState::EAS_Equipping;
}

//装备
void APlayerCharacter::Arm()
{
	//如果当前状态为可装备，则装备，播放装备动画
	PlayEquipMontage(FName("Equip"));
	//装备后角色状态设为单手装备
	CharacterState = ECharacterState::ECS_EquippedOneHanded;
	//设置当前动作状态为装备中
	ActionState = EActionState::EAS_Equipping;
}

//播放装备蒙太奇
void APlayerCharacter::PlayEquipMontage(const FName& SectionName)
{
	//获取动画实例
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		//播放装备动画
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);//跳转到指定段
	}
}

//死亡逻辑函数
void APlayerCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	//角色状态设为死亡
	ActionState = EActionState::EAS_Dead;
	//禁用网格体碰撞
	DisableMeshCollision();
}

//卸装(附在背上)
void APlayerCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		//将已装备武器附加到角色骨骼的背部插槽
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

//装备(附在手上)
void APlayerCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		//将已装备武器附加到角色骨骼的右手插槽
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RHSocket"));
	}
}

//完成装备
void APlayerCharacter::FinishEquipping()
{
	//完成装备后将当前动作状态设置为空闲
	ActionState = EActionState::EAS_Unoccupied;
}

//受击结束
void APlayerCharacter::HitReactEnd()
{
	//受击结束后将当前动作状态设置为空闲
	ActionState = EActionState::EAS_Unoccupied;
}

//初始化玩家UI
void APlayerCharacter::InitializePlayerOverlay()
{
	//获取玩家控制器
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		//获取玩家HUD
		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
		if (PlayerHUD)
		{
			//创建玩家覆盖UI
			PlayerOverlay = PlayerHUD->GetPlayerOverlay();
			if (PlayerOverlay && Attributes) //如果玩家覆盖UI和属性组件都存在
			{
				//设置血条百分比
				PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				//设置体力条百分比
				PlayerOverlay->SetStaminaBarPercent(1.0f);
				//设置金币数量
				PlayerOverlay->SetGold(0);
				//设置灵魂数量
				PlayerOverlay->SetSouls(0);
			}
		}
	}
}

//更新玩家UI的血量显示
void APlayerCharacter::SetHUDHealth()
{
	if (PlayerOverlay && Attributes) //如果玩家覆盖UI和属性组件都存在
	{
		//设置血条百分比
		PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

