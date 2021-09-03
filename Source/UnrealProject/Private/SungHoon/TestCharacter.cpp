// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/TestCharacter.h"
#include "SungHoon/SH_AnimInstance.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*------------------
			Camera
	--------------------*/
	// ������Ʈ ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SH_SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("SH_CAMERA"));

	// Attachment component
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	// component setting
	SpringArm->TargetArmLength = 600.0f; // distance
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f)); // rotation

	/*------------------
			Mesh
	--------------------*/
	// Add Mesh asset
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SH_FrostGiant(TEXT(
		"/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_FrostGiant.SK_CharM_FrostGiant"
	));
	if (SH_FrostGiant.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SH_FrostGiant.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f));

	/*------------------
		Animation
	--------------------*/
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); // A_BP ��� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> SH_Warrior_ANIM(TEXT(
		"/Game/SungHoon/Animation/SH_WarriorAnimBlueprint.SH_WarriorAnimBlueprint_C"
	));
	if (SH_Warrior_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SH_Warrior_ANIM.Class);
	}


	/////////////////
	// GTA ī�޶� ����
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

	/////////////////

	// ���� �Ǻ� �ʱ⺯�� �ʱ�ȭ
	bIsAttacking = false;

	// �����ϰ� ���� �ʱ�ȭ
	CurrentCombo = 0;
	MaxCombo = 4;
	bCanCombo = false;
	bNextComboInput = false;
}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// for movement
	PlayerInputComponent->BindAxis(TEXT("UpDown_SH"), this, &ATestCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight_SH"), this, &ATestCharacter::LeftRight);
	
	// for rotation
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATestCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATestCharacter::Turn);

	// for jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	// for Attack
	PlayerInputComponent->BindAction(TEXT("SH_Attack"), EInputEvent::IE_Pressed, this, &ATestCharacter::TestAttack);
}

void ATestCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// �ִ� �ν��Ͻ��� ������. (�ִϸ��̼� �������Ʈ�� ���� �������°�)
	Anim = Cast<USH_AnimInstance>(GetMesh()->GetAnimInstance());
	// ����Ǹ� �˾Ƽ� ���� �Ǻ� ������ false�� �ٲ���
	Anim->OnMontageEnded.AddDynamic(this, &ATestCharacter::OnTestAttackMontageEnded);

	// ��Ƽ���̵� ��������Ʈ ���ε�
	Anim->OnTestAttackHitCheck.AddLambda([this]()->void {
	
		// �޺� ���
		if (CurrentCombo < MaxCombo)
		{
			// �޺� �Է°��� �����
			bNextComboInput = true;
		}
	});

	// ��Ƽ���̵� ��������Ʈ ���ε�
	Anim->OnTestNextAttackCheck.AddLambda([this]()->void {

		// ���� �޺� ������ ��������.
		if (bCanCombo)
		{
			// �޺��� 1�� �ø���
			CurrentCombo++;
			// ���� ��Ÿ�� ������ �����.
			Anim->JumpToTestAttackMontageSection(CurrentCombo);

			// �ʱ�ȭ.
			bNextComboInput = false;
			bCanCombo = false;
		}
	});
}

void ATestCharacter::UpDown(const float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
}

void ATestCharacter::LeftRight(const float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void ATestCharacter::LookUp(const float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void ATestCharacter::Turn(const float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void ATestCharacter::TestAttack()
{
	// �̹� �������̸�
	if (bIsAttacking)
	{
		// �޺� ������ ����ϴ� ��Ȳ����
		if (bNextComboInput)
		{
			// �޺� ������ �����.
			bCanCombo = true;
		}
		// ������� �ʴ���
		else
		{
			// pass. ����
			return;
		}
	}
	// ���������� ������
	else
	{
		
		// ��Ÿ�� ù ���
		Anim->PlayTestAttackMontage();

		CurrentCombo++;
		bCanCombo = false;
		bNextComboInput = false;

		// ��Ÿ�� ��� �Ǻ� ���� true��
		bIsAttacking = true;
	}

}

// �ִϸ��̼��� ������
void ATestCharacter::OnTestAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	SH_LOG_S(Warning);
	// ���� �Ǻ� ���� �ʱ�ȭ
	bIsAttacking = false;

	// ���� ���� �ʱ�ȭ.
	CurrentCombo = 0;
	bNextComboInput = false;
	bCanCombo = false;

	Anim->SetCurrentSection();
}
