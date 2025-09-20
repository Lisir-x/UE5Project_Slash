

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
 	//����Ϊfalseʱ��ֹͣTick�¼�
	PrimaryActorTick.bCanEverTick = true;

	//�������ɱ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	//���õ��ɱ��������
	CameraBoom->bUsePawnControlRotation = true; //�����ɱ۸����������ת
	CameraBoom->TargetArmLength = 300.0f;	//���ɱ۳���
	CameraBoom->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f)); //���ɱ������ת

	//������������
	Camera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//����������������
	//��ɫ�������������ת
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//���ý�ɫ�ƶ��������
	GetCharacterMovement()->bOrientRotationToMovement = true; //��ɫ�����ƶ�����
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); //ÿ����ת����
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;	//��ɫ��������ٶ�
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f; //��ɫ��Сģ�������ٶ�

	//������������ײ����
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);	//������������Ϊ��̬����
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //����������ײ
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);//�赲�ɼ���ͨ��
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);//�ص���̬����ͨ��
	GetMesh()->SetGenerateOverlapEvents(true);	//�������ص��¼�

}

//ÿһ֡������
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Attributes && PlayerOverlay)
	{
		//���������ָ�����
		Attributes->RegenStamina(DeltaTime);
		//����UI�����ٷֱ�
		PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

//���ڰ�����
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//���ƶ�
		EnhancedInputComponent->BindAction(IA_Move,
			ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		//���ӽ�
		EnhancedInputComponent->BindAction(IA_Look,
			ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		//����Ծ
		EnhancedInputComponent->BindAction(IA_Jump,
			ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		//��ʰȡ��װ��
		EnhancedInputComponent->BindAction(IA_Equip,
			ETriggerEvent::Started, this, &APlayerCharacter::FKeyPressed);
		//�󶨹���
		EnhancedInputComponent->BindAction(IA_Attack,
			ETriggerEvent::Started, this, &APlayerCharacter::Attack);
		//������
		EnhancedInputComponent->BindAction(IA_Dodge,
			ETriggerEvent::Started, this, &APlayerCharacter::Dodge);

	}
}

//��Ծ����
void APlayerCharacter::Jump()
{
	//ֻ���ڿ���״̬�²�����Ծ
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

//���˺���
float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	//���ô����˺�����
	HandleDamage(DamageAmount);
	//����UIѪ��
	SetHUDHealth();

	return DamageAmount;
}

//���к������ܵ��˺�ʱ����
void APlayerCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	//����������ײ
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	//���������������Ѫ������0
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		//���õ�ǰ����״̬Ϊ�ܻ���Ӧ
		ActionState = EActionState::EAS_HitReaction;
	}

}

//���õ�ǰ�ص�����Ʒ
void APlayerCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

//������
void APlayerCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && PlayerOverlay)
	{
		//�����굽�������
		Attributes->AddSouls(Soul->GetSouls());
		//����UI�������
		PlayerOverlay->SetSouls(Attributes->GetSouls());
	}
}

//��ӻƽ�
void APlayerCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && PlayerOverlay)
	{
		//��ӻƽ��������
		Attributes->AddGold(Treasure->GetGold());
		//����UI�ƽ�����
		PlayerOverlay->SetGold(Attributes->GetGold());
	}
}

//��Ϸ��ʼ������ʱ����
void APlayerCharacter::BeginPlay()
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
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	//Ϊ��ɫ��ӱ�ǩ
	Tags.Add(FName("EngageableTarget"));
	//��ʼ�����UI
	InitializePlayerOverlay();

}

//�ƶ����봦��
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	//��ȡ����Ķ�ά����
	FVector2D MoveVector = Value.Get<FVector2D>();
	//������ڷǿ���״̬���ƶ�
	if (!IsUnoccupied()) return;
	if (Controller)	//����п�����
	{
		//��ȡ����������ת
		const FRotator ControlRotation = Controller->GetControlRotation();
		//ֻ��ȡƫ����(Yaw)
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		//����ƫ���ǻ�ȡǰ����(Y)���ҷ���(X)
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//��������������X��Y�����ֱ���ǰ����(Y)���ҷ���(X)�ƶ���ɫ
		AddMovementInput(ForwardDirection, MoveVector.Y);
		AddMovementInput(RightDirection, MoveVector.X);
		//�жϹ�������
		StopAttackMontage();
	}
	//FVector InputVector = FVector(MoveVector, 0.0f);
	//GetPawn()->AddMovementInput(InputVector);
}

//�ӽ����봦��
void APlayerCharacter::Look(const FInputActionValue& Value)
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

//F�����봦��
void APlayerCharacter::FKeyPressed()
{
	//���Խ���ǰ��ʰȡ����Ʒת��Ϊ����
	ABaseWeapon* OverlappingWeapon = Cast<ABaseWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			//�����װ��������������
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon); //װ������
	}
	else //��û�п�ʰȡ����Ʒ����ִ��жװ��װ������
	{
		if (CanDisarm())
		{
			Disarm(); //жװ
		}
		else if (CanArm())
		{
			Arm(); //װ��
		}
	}
}

//����
void APlayerCharacter::Attack()
{
	Super::Attack();

	//����ǰ����״̬Ϊ���У��򲥷Ź�������
	if (CanAttack())
	{
		PlayAttackMontage();
		//���õ�ǰ����״̬Ϊ������
		ActionState = EActionState::EAS_Attacking;
	}
}

//����
void APlayerCharacter::Dodge()
{
	//����ǰ����״̬��Ϊ���л��������㣬��ִ������
	if (!IsUnoccupied() || !HasEnoughStamina()) return;
	//�������ܶ���
	PlayDodgeMontage();
	//���õ�ǰ����״̬Ϊ����
	ActionState = EActionState::EAS_Dodge;

	if (Attributes && PlayerOverlay)
	{
		//����������������
		Attributes->UseStamina(Attributes->GetDodgeCost());
		//����UI�����ٷֱ�
		PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

//װ������
void APlayerCharacter::EquipWeapon(ABaseWeapon* Weapon)
{
	//���п�ʰȡ����������װ��������
	Weapon->Equip(GetMesh(), FName("RHSocket"), this, this);
	//��ɫ״̬��Ϊ����װ��
	CharacterState = ECharacterState::ECS_EquippedOneHanded;
	//����ǰ��ʰȡ��������Ϊ��װ������
	EquippedWeapon = Weapon;
}

//��������
void APlayerCharacter::AttackEnd()
{
	//���������󽫶���״̬����Ϊ����
	ActionState = EActionState::EAS_Unoccupied;
}

//���ܽ���
void APlayerCharacter::DodgeEnd()
{
	//���ܽ����󽫶���״̬����Ϊ����
	ActionState = EActionState::EAS_Unoccupied;
}

//�ж��Ƿ���Թ���
bool APlayerCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

//�ж��Ƿ����жװ
bool APlayerCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

//�ж��Ƿ����װ��
bool APlayerCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped && 
		EquippedWeapon;
}

//�ж��Ƿ����㹻����
bool APlayerCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

//�жϵ�ǰ�Ƿ����״̬
bool APlayerCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

//жװ
void APlayerCharacter::Disarm()
{
	//�����ǰ״̬Ϊ��жװ����жװ������жװ����
	PlayEquipMontage(FName("Unequip"));
	//жװ���ɫ״̬��Ϊδװ��
	CharacterState = ECharacterState::ECS_Unequipped;
	//���õ�ǰ����״̬Ϊװ����
	ActionState = EActionState::EAS_Equipping;
}

//װ��
void APlayerCharacter::Arm()
{
	//�����ǰ״̬Ϊ��װ������װ��������װ������
	PlayEquipMontage(FName("Equip"));
	//װ�����ɫ״̬��Ϊ����װ��
	CharacterState = ECharacterState::ECS_EquippedOneHanded;
	//���õ�ǰ����״̬Ϊװ����
	ActionState = EActionState::EAS_Equipping;
}

//����װ����̫��
void APlayerCharacter::PlayEquipMontage(const FName& SectionName)
{
	//��ȡ����ʵ��
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		//����װ������
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);//��ת��ָ����
	}
}

//�����߼�����
void APlayerCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	//��ɫ״̬��Ϊ����
	ActionState = EActionState::EAS_Dead;
	//������������ײ
	DisableMeshCollision();
}

//жװ(���ڱ���)
void APlayerCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		//����װ���������ӵ���ɫ�����ı������
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

//װ��(��������)
void APlayerCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		//����װ���������ӵ���ɫ���������ֲ��
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RHSocket"));
	}
}

//���װ��
void APlayerCharacter::FinishEquipping()
{
	//���װ���󽫵�ǰ����״̬����Ϊ����
	ActionState = EActionState::EAS_Unoccupied;
}

//�ܻ�����
void APlayerCharacter::HitReactEnd()
{
	//�ܻ������󽫵�ǰ����״̬����Ϊ����
	ActionState = EActionState::EAS_Unoccupied;
}

//��ʼ�����UI
void APlayerCharacter::InitializePlayerOverlay()
{
	//��ȡ��ҿ�����
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		//��ȡ���HUD
		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
		if (PlayerHUD)
		{
			//������Ҹ���UI
			PlayerOverlay = PlayerHUD->GetPlayerOverlay();
			if (PlayerOverlay && Attributes) //�����Ҹ���UI���������������
			{
				//����Ѫ���ٷֱ�
				PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				//�����������ٷֱ�
				PlayerOverlay->SetStaminaBarPercent(1.0f);
				//���ý������
				PlayerOverlay->SetGold(0);
				//�����������
				PlayerOverlay->SetSouls(0);
			}
		}
	}
}

//�������UI��Ѫ����ʾ
void APlayerCharacter::SetHUDHealth()
{
	if (PlayerOverlay && Attributes) //�����Ҹ���UI���������������
	{
		//����Ѫ���ٷֱ�
		PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}
