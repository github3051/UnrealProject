// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_Character.h"
#include "SungHoon/SH_AnimInstance.h" // Added SH_AnimInstance, for Attack Montage
#include "SungHoon/SH_Weapon.h"
#include "SungHoon/SH_CharacterStatComponent.h" // ���漱���Ѱ� ��üȭ
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "SungHoon/SH_CharacterWidget.h"
#include "SungHoon/SH_AIController.h"
#include "SungHoon/SH_GameInstance.h"
#include "SungHoon/SH_PlayerController.h"
#include "SungHoon/SH_PlayerState.h"
#include "SungHoon/SH_HUDWidget.h"
#include "SungHoon/SH_GameModeBase.h"


/*---------------------------------
		UnrealProjectSetting
----------------------------------*/
// UnrealProjectSetting ��⿡ �ִ°� �߰�
#include "SH_CharacterSetting.h" 


// Sets default values
ASH_Character::ASH_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Print LOG
	SH_LOG(Warning, TEXT("Execute SH_Character Constructor!"));


	// Create component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SH_SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("SH_CAMERA"));

	// Attachment
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	// Set up values
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f; // distance
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f)); // rotation

	// Add Mesh asset
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SH_CARDBOARD(TEXT(
		"/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"
	));
	if (SH_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SH_CARDBOARD.Object);
	}


	// Add Animation asset (AnimationBlueprint, UAnimInstance)
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); // A_BP ��� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> SH_CARDBOARD_ANIM(TEXT(
		"/Game/SungHoon/Animation/SH_WarriorAnimBlueprint.SH_WarriorAnimBlueprint_C"
	));
	if (SH_CARDBOARD_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SH_CARDBOARD_ANIM.Class);
	}

	// Set default ��Ʈ�� ��� ����. DIABLO ���
	// SetControlMode(EControlMode::GTA); // -> Possessedby���� �ʱ�ȭ

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f; // ȸ�� �ӵ�

	// for jump speed
	GetCharacterMovement()->JumpZVelocity = 450.0f;

	// for attack's montage
	IsAttacking = false;

	// for attack combo
	MaxCombo = 4;
	AttackEndComboState(); // �޺��� ���� ���·� �ʱ�ȭ

	// Set Collision channel
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SH_Character"));

	// for Debug Drawing by Capsule
	AttackRange = 80.0f; // ���� ����
	AttackRadius = 50.0f; // ������

	// CharacterStat ������Ʈ ����.
	CharacterStat = CreateDefaultSubobject<USH_CharacterStatComponent>(TEXT("SH_CHARACTERSTAT"));

	/*--------------------------------
					UI
	---------------------------------*/
	// HPBar UI ����
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SH_HPBARWIDGET"));
	// �޽ÿ� ����
	HPBarWidget->SetupAttachment(GetMesh());
	// ȭ����� ���� ��ġ ����
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	// �׻� �÷��̾��� ȭ���� ���� ������ Screen ��� ����.
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	// �ּ� ��������. ���´� UUserWidget�̴�. Ŭ��������. ��ο� _C �־��ֱ�.
	static ConstructorHelpers::FClassFinder<UUserWidget> SH_UI_HUD(TEXT(
		"/Game/SungHoon/UI/SH_HPBar.SH_HPBar_C"));
	if (SH_UI_HUD.Succeeded())
	{
		// �ּ��� ������. ������ Ŭ������.
		HPBarWidget->SetWidgetClass(SH_UI_HUD.Class);
		// ������ ũ�� ����
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	/*--------------------------------
					AI
	---------------------------------*/
	// AI ��Ʈ�ѷ� �������� SH_AIController Ŭ������ ����
	AIControllerClass = ASH_AIController::StaticClass();
	// �ڵ� AI �������� ���忡 �̸� ��ġ�ǰų� ���� �����Ǵ� ���͵��� �ǹ���.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	// CardBoard ���� ��ȣ
	AssetIndex = 3;
	// ���� ĳ���͸� ����
	SetActorHiddenInGame(true);
	// UI�� ����
	HPBarWidget->SetHiddenInGame(true);
	// ������ �����ӿ�ũ ��
	SetCanBeDamaged(false);

	// ���� ���� �ð� 5��
	DeadTimer = 5.0f;
}

// Called when the game starts or when spawned
void ASH_Character::BeginPlay()
{
	Super::BeginPlay();

	/*
		�𸮾� 4.21 ���ķδ� ������ �ʱ�ȭ ������ PostInitializeComponents���� BeginPlay�� �����.
		�ùٸ��� �����Ϸ��� �������� BeginPlay�� �߰��ϵ��� ����.
	*/

	// ĳ���� ���� ��������Ʈ ����. HPBar ������ ����Ǿ��ִ� (�θ�) ���� Ŭ���� ������ ������. (������ �ִ��ν��Ͻ� ��������)
	auto CharacterWidget = Cast<USH_CharacterWidget>(HPBarWidget->GetUserWidgetObject());
	// ��ȿ�ϴٸ�
	if (CharacterWidget != nullptr)
	{
		// ĳ���Ͱ� ����ִ� CharacterStat ������ �ѱ�.
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

	// �÷��̾� ��Ʈ�ѷ� ���� ������
	bIsPlayer = IsPlayerControlled();


	// �÷��̾���
	if (bIsPlayer)
	{
		// �÷��̾� ��Ʈ�ѷ� ��������
		SHPlayerController = Cast<ASH_PlayerController>(GetController());
		// ��Ʈ�ѷ� ���� ����� ĳ�����ؼ� �����Դٸ� ���
		SH_CHECK(SHPlayerController != nullptr);
	}
	// �÷��̾ �ƴ϶�� = AI���
	else
	{
		// AI ��Ʈ�ѷ� ��������
		SHAIController = Cast<ASH_AIController>(GetController());
		// ��Ʈ�ѷ� ���� ����� ĳ�����ؼ� �����Դٸ� ���
		SH_CHECK(SHAIController != nullptr);
	}

	// USH_CharacterSetting�� ���� CDO �о��
	auto DefaultSetting = GetDefault<USH_CharacterSetting>();

	if (bIsPlayer)
	{
		AssetIndex = 3;
	}
	else
	{
		// ini���� �о�� ����. �迭�� 0�� �ε������� �����ϹǷ� -1 ����.
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	// �ּ� ����
	{
		// �ּ� ��θ� �ϳ� �о��
		CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
		// ���� ������Ʈ ������ ���� �ν��Ͻ��� ������. �װ� ĳ���� (��� �߰� �ʿ�)
		auto SHGameInstance = Cast<USH_GameInstance>(GetGameInstance());
		// �������̶��
		if (SHGameInstance != nullptr)
		{
			// �񵿱� ������� ���� �����߿��� ��ο� �ִ� �ּ��� ��û��. �׸��� ��������Ʈ ���ε��� �Լ� ������.
			AssetStreamingHandle = SHGameInstance->StreamableManager.RequestAsyncLoad(
				CharacterAssetToLoad, FStreamableDelegate::CreateUObject(
					this, &ASH_Character::OnAssetLoadCompleted));
		}

		// ���� ĳ���� state�� LOADING���� ����
		SetCharacterState(ESH_CharacterState::LOADING);
	}
}

// ĳ������ State�� ����
void ASH_Character::SetCharacterState(ESH_CharacterState NewState)
{
	// ���� state�� ���ο� state�� �ٸ����� ���
	SH_CHECK(CurrentState != NewState);
	CurrentState = NewState;

	// ���� ���¿� ���� �з�
	switch (CurrentState)
	{
	case ESH_CharacterState::PREINIT:
		break;
	case ESH_CharacterState::LOADING:
	{
		// �÷��̾� ���
		if (bIsPlayer)
		{
			// �÷��̾��� ������ ����
			DisableInput(SHPlayerController);

			// CharacterStat ������ HUD�� ������.
			SHPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			// �÷��̾� ������Ʈ ������
			auto SHPlayerState = Cast<ASH_PlayerState>(GetPlayerState());
			SH_CHECK(SHPlayerState != nullptr);
			// �÷��̾��� ������ ���Ӱ� ���� ����.
			CharacterStat->SetNewLevel(SHPlayerState->GetCharacterLevel());
		}
		// AI���
		else
		{
			// ���� ��带 ������. ���� �������϶� ���� ��带 �������� �Լ�.
			auto SHGameMode = Cast<ASH_GameModeBase>(GetWorld()->GetAuthGameMode());
			SH_CHECK(SHGameMode != nullptr);
			// ���� ��忡�� Score���� ������ 0.8�� ���� ���� AI ������ ����
			int32 TargetLevel = FMath::CeilToInt(((float)SHGameMode->GetScore()* 0.8f));
			// TargetLevle�� 1~20 ������ ������ Ȯ������. 
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			SH_LOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			// ĳ���� �������ΰ� �ش� ������ ���� ��������.
			CharacterStat->SetNewLevel(FinalLevel);
		}

		// ���͸� ����
		SetActorHiddenInGame(true);
		// UI ����
		HPBarWidget->SetHiddenInGame(true);
		// ������ �����ӿ�ũ ��
		SetCanBeDamaged(false);
		break;
	}
	case ESH_CharacterState::READY:
	{
		// ���� ���̱�
		SetActorHiddenInGame(false);
		// UI ���̱�
		HPBarWidget->SetHiddenInGame(false);
		// ������ �����ӿ�ũ �۵�
		SetCanBeDamaged(true);

		// ĳ���� ������ �׾������� ��������Ʈ �Լ��� ���ε���.
		CharacterStat->OnHPIsZero.AddLambda([this]()->void {
			// ���� ���·� �ٲ�. �ڱ��ڽ� ȣ���μ�.
			SetCharacterState(ESH_CharacterState::DEAD);
		});

		// HPBarWidget Ŭ���� ������ ������
		auto CharacterWidget = Cast<USH_CharacterWidget>(HPBarWidget->GetUserWidgetObject());
		// ����� ���������� �����Դٸ� ���
		SH_CHECK(CharacterWidget != nullptr);
		// ĳ���� ���� ������ Character Stat�� ��������
		CharacterWidget->BindCharacterStat(CharacterStat);

		// �÷��̾���
		if (bIsPlayer)
		{
			// GTA���� ����
			SetControlMode(EControlMode::GTA);
			// ������ �ӵ� ����
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			// ���� ���
			EnableInput(SHPlayerController);
		}
		// AI���
		else
		{
			// NPC ���� ����
			SetControlMode(EControlMode::NPC);
			// ������ �ӵ� ����
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			// �����̺��Ʈ�� ����
			SHAIController->RunAI();
		}

		break;
	}
	case ESH_CharacterState::DEAD:
	{
		// �ش� ĳ������ �浹ó���� ��
		SetActorEnableCollision(false);
		// Ȥ�� �޽ð� �����ִٸ� Ŵ
		GetMesh()->SetHiddenInGame(false);
		// HP UI ����
		HPBarWidget->SetHiddenInGame(true);
		// ���� �ִϸ��̼� �۵�
		SHAnim->SetDeadAnim();
		// ������ �����ӿ�ũ ��
		SetCanBeDamaged(false);

		// �÷��̾���
		if (bIsPlayer)
		{
			// ���� ����
			DisableInput(SHPlayerController);
		}
		// AI���
		else
		{
			// �����̺�� Ʈ�� ����
			SHAIController->StopAI();
		}

		// Ÿ�̸� ����. DeadTimer ���Ŀ� �����Լ� �����.
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {

			// �÷��̾���
			if (bIsPlayer)
			{
				// ������ �ƿ� �ٽ� �����ض�
				SHPlayerController->RestartLevel();
			}
			// AI���
			else
			{
				// ������Ʈ �ı�
				Destroy();
			}

		}), DeadTimer, false);

		break;
	}
	default:
		break;
	}
}

ESH_CharacterState ASH_Character::GetCharacterState() const
{
	return CurrentState;
}

// Setting View
void ASH_Character::SetControlMode(const EControlMode& NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		SH_LOG(Error, TEXT("GTA Mode"));

		//SpringArm->TargetArmLength = 1000.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator); // ���� ������ ������.
		ArmLengthTo = 600.0f; // distance
		SpringArm->bUsePawnControlRotation = true; // ��Ʈ�� ȸ������ �����ϴ�.
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bInheritRoll = true;

		// ī�޶�� ���� �浹�� üũ����. ���� ������ �˾Ƽ� ���εʡ�
		SpringArm->bDoCollisionTest = true;

		// Pawn ������ ���ۺ��� ȸ�� ����. (���� ��� ����. default����)
		bUseControllerRotationYaw = false;

		// CharacterMovement�� �ִ� ����� �̿�����.
		// ī�޶� �����̴� �������� �޽ø� �����̰� �ϴ� �����. true ����. (���� false��)
		GetCharacterMovement()->bOrientRotationToMovement = true;

		// 45���� ���ܼ� ȸ���ϴ°��� �ذ��ϱ� ����, �ε巴�� ȸ���ϴ� ���. GTA�� ������ �����Ƿ� ��������.
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ȸ���ӵ� ����
		break;

	case EControlMode::DIABLO:
		SH_LOG(Error, TEXT("DIABLO Mode"));

		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f,0.0f,0.0f)); // �Ʒ��� ����.
		ArmLengthTo = 900.0f; // distance
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f); // ī�޶� ����
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bInheritRoll = false;

		// ī�޶�� ���� �浹�� üũ����. ���� ������ �˾Ƽ� ���εǴ°� ��.
		SpringArm->bDoCollisionTest = false;

		// Pawn ������ ���ۺ��� ȸ�� ����. (���� ��� ����)
		// ��� �ؿ��� bUseControllerDesiredRotation�� �Ѽ� �ذ���. �Ѵ� ���������� �ȵȵ�!
		bUseControllerRotationYaw = false;

		// CharacterMovement�� �ִ� ����� �̿�����.
		// ī�޶� �����̴� �������� �޽ø� �����̰� �ϴ� �����. false. ������ �����̹Ƿ�.
		GetCharacterMovement()->bOrientRotationToMovement = false;

		// 45���� ���ܼ� ȸ���ϴ°��� �ذ��ϱ� ����, �ε巴�� ȸ���ϴ� ���. ������.
		GetCharacterMovement()->bUseControllerDesiredRotation = true; // �ε巯�� ȸ���� ����

		// ĳ���� �޽��� ȸ���ӵ� ���� bUseControllerDesiredRotation�� ��ȯ �ӵ��� �ǹ�.
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;

	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		// ȸ�� �ӵ�
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;

	default:
		break;
	}
}

// Called every frame
void ASH_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �� �����Ӹ��� ī�޶� ������ �Ÿ� ���
	// InterpTo(���� ��, ���� ��ǥ��, �ð�, �����ӵ�)
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		break;

	case EControlMode::DIABLO:
		/*
			DIABLO ����� �Է°��� �ް� �ٷ� �����̰� ȸ���ϴ°� �ƴ϶�,
			�ش��ϴ� ȸ���� �����Ӱ��� ��� ó���� �ڿ�, ���������� Tick����
			������� �̿��ؼ� �̵��ϰ� ȸ���ϵ��� ����. �׷��� ���� ��������
			Tick�Լ��� �ִ�. GTA�� �ƴ�. GTA�� �Է��Լ��� ����.
		*/

		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));

		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			// rotation (�޽ð� ���ư�) 
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			// move (���� �̵�)
			AddMovementInput(DirectionToMove);
		}
		break;

	default:
		break;
	}

}

// Called to bind functionality to input
void ASH_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// for movement
	PlayerInputComponent->BindAxis(TEXT("UpDown_SH"), this, &ASH_Character::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight_SH"), this, &ASH_Character::LeftRight);

	// for rotation
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ASH_Character::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASH_Character::Turn);

	// for changing view button
	PlayerInputComponent->BindAction(TEXT("SH_ViewChange"), EInputEvent::IE_Pressed, this, &ASH_Character::ViewChange);

	// for jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	// for Attack
	PlayerInputComponent->BindAction(TEXT("SH_Attack"), EInputEvent::IE_Pressed, this, &ASH_Character::Attack);
}

void ASH_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// ��������Ʈ ����� ���� �ִ��ν��Ͻ��� �ӽ÷� �����´�.
	SHAnim = Cast<USH_AnimInstance>(GetMesh()->GetAnimInstance());

	// �����̸� ��ũ�� ���
	SH_CHECK(SHAnim != nullptr);

	// �ִ��ν��Ͻ��� �ִ� �Լ��� ĳ���� Ŭ������ dynamic ��������Ʈ ���.
	SHAnim->OnMontageEnded.AddDynamic(this, &ASH_Character::OnAttackMontageEnded);

	// ��Ÿ�� �ִϸ��̼ǿ��� NextAttackCheck ��Ƽ���̰� ����Ǹ� ȣ���ϴ� ��������Ʈ �Լ�(�̺�Ʈ)
	// �ִ� �ν��Ͻ����� OnAttackCheck.BroadCast�� �ϸ� �����Լ��� ȣ���.
	// Multicast ��������Ʈ
	SHAnim->OnNextAttackCheck.AddLambda([this]()->void {

		SH_LOG(Warning, TEXT("OnNextAttackCheck Lambda Function Called! CurrentCombo : %d"), CurrentCombo);

		CanNextCombo = false;
		// �޺������� ������ Ÿ�ֿ̹� ���Դٸ�
		if (IsComboInputOn)
		{
			AttackStartComboState();
			SHAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	// ������ ���� ��������Ʈ ���. ��Ÿ�� �ִϸ��̼��� AttackHitCheck ��Ƽ���� ����� �ڵ� ȣ��
	// Muticast ��������Ʈ
	SHAnim->OnAttackHitCheck.AddUObject(this, &ASH_Character::AttackCheck);

	// �״°Ϳ� ���� �̺�Ʈ�� ����ϱ� ���� ��������Ʈ ���!
	CharacterStat->OnHPIsZero.AddLambda([this]()->void {

		SH_LOG(Error, TEXT("OnHPIsZero Delegate function call!"));
		// ���� ����
		SHAnim->SetDeadAnim();
		// �浹ó�� ����
		SetActorEnableCollision(false);
	});


}

// ����ġ ȹ�淮 ��ȯ
int32 ASH_Character::GetExp() const
{
	return CharacterStat->GetDropExp();
}

// ���� ���� ���� �Լ�
float ASH_Character::GetFinalAttackRange() const
{
	// ���Ⱑ ������ ���� ���� ����, �ƴϸ� �⺻ ĳ���� ���� ������ ����.
	return (CurrentWeapon != nullptr) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

// ���� ���� ������ �� ��� �Լ�
float ASH_Character::GetFinalAttackDamage() const
{
	// ���Ⱑ �������� �������� ������ �����.
	float AttackDamage = (CurrentWeapon != nullptr) ?
		(CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : CharacterStat->GetAttack();
	// ���Ⱑ �������� ������
	float AttackModifer = (CurrentWeapon != nullptr) ? CurrentWeapon->GetAttackModifier() : 1.0f;

	// ���� ���ݷ��� ���� ���ݷ� * �߰� ������ ���갪
	return AttackDamage * AttackModifer;
}

float ASH_Character::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// ���������� �Ѿ���� ��������.
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	SH_LOG(Warning, TEXT("Actor : %s took damage : %f"), *GetName(), FinalDamage);

	// ���� �������� ���ȿ��� ��������.
	CharacterStat->SetDamage(FinalDamage);

	// ���� ���°� �����̶��
	if (CurrentState == ESH_CharacterState::DEAD)
	{
		// ����ڰ� �÷��̾���
		if (EventInstigator->IsPlayerController())
		{
			// �÷��̾��� ��Ʈ�ѷ��� ������.
			auto Instigator_PlayerController = Cast<ASH_PlayerController>(EventInstigator);
			SH_CHECK(Instigator_PlayerController != nullptr, 0.0f);
			// NPC ������ ó��. ����ġ ����.
			Instigator_PlayerController->NPCKill(this);
		}
	}
	return FinalDamage;
}

// ���� ������ ȣ��. ��Ʈ�ѷ��� ���� ���������.
void ASH_Character::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	// ���� �ǰ� �α� ����
	SH_LOG_S(Warning);

	// �÷��̾���
	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::GTA);
		// �ִ� �̵� �ӵ�
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	// AI ���
	else
	{
		// NPC ����
		SetControlMode(EControlMode::NPC);
		// �ִ� �̵� �ӵ�
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

bool ASH_Character::CanSetWeapon()
{
	// ���Ⱑ ���ٸ� true
	//return (CurrentWeapon == nullptr);

	// �׻� ��ü�� �� �ְ� �����Ѵ�. chapter 14
	return true;
}

void ASH_Character::SetWeapon(ASH_Weapon* NewWeapon)
{
	// ���� �ּ������� �� �������� ���
	SH_CHECK(NewWeapon != nullptr);

	// ���Ⱑ �̹� �ִٸ� �����ϰ� �ı���.
	if (CurrentWeapon != nullptr)
	{
		// ���Ϳ� ����� �κ��� ������. Attachment �Լ��� ������
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy(); // ���⸦ �ı���.
		CurrentWeapon = nullptr; // �����Ͱ� �ʱ�ȭ ����.
	}

	// ���� ���̷�Ż �޽��� ���ϸ�� ��Ȯ�� ��ġ�ؾ���.
	FName WeaponSocket(TEXT("hand_rSocket")); // string ����
	// �޽ÿ� �ش��ϴ� ���ϸ��� �����Ѵٸ�
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		// ���� ���͸� ĳ���� �Žÿ� ����.
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		// ���� ���͸� �� ĳ���� Ŭ������ ��ӽ�Ŵ.
		NewWeapon->SetOwner(this);
		// ���� ���� ���Ϳ� ���� Ŭ������ ���� �����͸� ����
		CurrentWeapon = NewWeapon;
	}
}

// for forward, back move
void ASH_Character::UpDown(const float NewAxisValue)
{
	// AddMovementInput(GetActorForwardVector(), NewAxisValue);
	// AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//AddMovementInput(GetActorForwardVector(), NewAxisValue); // �޽ð� �ٶ󺸴� ���� �������� ������.

		// ���� ��Ʈ�ѷ��� ȸ������ �����ͼ� �������ͷ� ������ ���ؿ�. �� �������� �̵���.
		//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		break;

	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;

	default:
		break;
	}
}

// for left, right move
void ASH_Character::LeftRight(const float NewAxisValue)
{
	// AddMovementInput(GetActorRightVector(), NewAxisValue);
	// AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//AddMovementInput(GetActorRightVector(), NewAxisValue); // �޽��� ������ ������ �������� ������.

		// ���� ��Ʈ�ѷ��� ȸ������ �����ͼ� �������ͷ� ������ ���ؿ�. �� �������� �̵���.
		//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;

	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;

	default:
		break;
	}
}

// Pitch (���Ʒ�)
void ASH_Character::LookUp(const float NewAxisValue)
{
	// ���Ʒ� ȸ���� Y�� ���� ȸ��(���� ��)
	// AddControllerPitchInput(NewAxisValue);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;

	case EControlMode::DIABLO:
		break;

	default:
		break;
	}
}

// Yaw (�¿� ���ۺ���)
void ASH_Character::Turn(const float NewAxisValue)
{
	// ������ ȸ�� Z�� ���� ȸ�� (���� ��)
	// AddControllerYawInput(NewAxisValue);
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;

	case EControlMode::DIABLO:
		break;

	default:
		break;
	}
}

void ASH_Character::ViewChange()
{
	// Print LOG
	SH_LOG_S(Error);

	switch (CurrentControlMode)
	{
		// ���� ��尡 GTA���
	case EControlMode::GTA:
		// for DIABLO
		/*
			�Ʒ� ��Ʈ�ѷ��� ȸ������ �������ִ� ������ ����?
		*/
		// Controller�� rotation���� ���� ������ Rotation������ ����.
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);;
		break;

		// ���� ��尡 DIABLO���
	case EControlMode::DIABLO:
		// for GTA
		/*
			DIABLO�� SpringArm�� �������� ȸ���ϱ� ����.
			�̰� �ٽ� ��Ʈ�ѷ��� ȸ�������� �������� �ʿ䰡 ����.
			�̰� �����ָ� GTA�� ��ȯ�Ҷ� �׻� Default ī�޶� ��������,
			���鼦�� ����.
			��, ī�޶��� ���� ������ �״�� �����´ٸ�, ���� �ڿ����������°���.
		*/
		// ���� SpringArm�� ȸ������ ControlRotation ������ ����.
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::GTA);;
		break;

	default:
		break;
	}
}

void ASH_Character::Attack()
{
	// ���콺 ���� Ŭ���Ҷ����� �����.
	SH_LOG_S(Error);

	// �������̸� ��ȯ
	if (IsAttacking)
	{
		SH_CHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));

		// ���� �޺��� ������ �� �ִٸ�
		if (CanNextCombo)
		{
			// �޺� ���踦 true�� ���ش�.
			IsComboInputOn = true;
		}
	}
	else
	{
		// CurrentCombo�� ����� �ʱ�ȭ �ƴ��� üũ
		SH_CHECK(CurrentCombo == 0);
		AttackStartComboState(); // ���� �޺� ����.
		// �ִ� �ν��Ͻ��� �Լ��� ĳ���Ϳ��� ȣ����.
		SHAnim->PlayAttackMontage(); // ���� ��Ÿ�ָ� ����.
		SHAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true; // ������
	}
}

// ���̳��� ��������Ʈ �Լ�. ���� ��Ÿ�ְ� ������ �������ֱ� �ٶ�� �Լ�.
void ASH_Character::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	SH_CHECK(IsAttacking);
	SH_CHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	// ���� �ִϸ��̼��� ������ ���õ� ��� �Լ��� ȣ����.
	OnAttackEnd.Broadcast();
}

void ASH_Character::AttackStartComboState()
{
	// ���� �ܰ�� ����
	CanNextCombo = true;
	// �Է°��� �ʱ�ȭ
	IsComboInputOn = false;

	// bool(0 <= CurrentCombo <= MaxCombo - 1 (=3))
	SH_CHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));

	// 1 <= CurrentCombo + 1 <= MaxCombo(=4)
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ASH_Character::AttackEndComboState()
{
	// ���� �ʱ�ȭ
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ASH_Character::AttackCheck()
{
	// ���� ������ ������
	float FinalAttackRange = GetFinalAttackRange();


	// �浹ó�� ����� ���� ����ü ���� ����
	FHitResult HitResult;

	// Ž�� ����� ���� ���� �Ķ����
	FCollisionQueryParams Params(NAME_None, false, this);

	// �浹üũ ���� bool ����
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius), //��ü
		Params // �Ķ����. �� �ڽ��� Ž������ ����.
	);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	// ĸ���� �߾Ӱ�
	FVector Center = GetActorLocation() + TraceVec * 0.5f; // ����
	// ĸ���� ���� ����
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
	// ĸ���� Z���� ���� ����. ���ʹϾ����� �ٲ�
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	// �浹�� ������ �ʷ�, ������ ����
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	// ȭ�鿡 ���̴� �ð�
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif

	if (bResult)
	{
		// �浹�� ����� ���Ͷ��, GC�� ���� �������� �ʾҴٸ�(��������� �ʾƼ�)
		if (HitResult.Actor.IsValid())
		{
			SH_LOG(Error, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}

// BeginPlay���� ��ϵ� ��������Ʈ �Լ�. �ε��� �޽� �ּ��� ����ϴ� �Լ�
void ASH_Character::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());

	// ������ ���� ��Ŵ. Shared �����ʹ� �ѹ� ��������� ���� �ʱ�ȭ �������.
	AssetStreamingHandle.Reset();
	// ���� �ּ� ������ ���������� ���.
	SH_CHECK(AssetLoaded != nullptr);
	// �޽ÿ� �����. ���� �����ڿ�
	GetMesh()->SetSkeletalMesh(AssetLoaded);
	// ���� STATE�� READY ���·� �ٲ�.
	SetCharacterState(ESH_CharacterState::READY);
}
