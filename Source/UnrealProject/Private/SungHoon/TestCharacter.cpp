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
	// 컴포넌트 생성
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
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); // A_BP 모드 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> SH_Warrior_ANIM(TEXT(
		"/Game/SungHoon/Animation/SH_WarriorAnimBlueprint.SH_WarriorAnimBlueprint_C"
	));
	if (SH_Warrior_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SH_Warrior_ANIM.Class);
	}


	/////////////////
	// GTA 카메라 설정
	SpringArm->bUsePawnControlRotation = true; // 컨트롤 회전값과 동일하다.
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// 카메라와 벽의 충돌을 체크해줌. 벽이 있으면 알아서 줌인됨★
	SpringArm->bDoCollisionTest = true;

	// Pawn 설정의 빙글빙글 회전 막음. (기존 방식 막음. default였음)
	bUseControllerRotationYaw = false;

	// CharacterMovement에 있는 기능을 이용하자.
	// 카메라가 움직이는 방향으로 메시를 움직이게 하는 기능임. true 설정. (원래 false임)
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 45도씩 끊겨서 회전하는것을 해결하기 위해, 부드럽게 회전하는 기능. GTA는 끊기지 않으므로 내버려둠.
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // 회전속도 조절

	/////////////////

	// 공격 판별 초기변수 초기화
	bIsAttacking = false;

	// 안전하게 최초 초기화
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

	// 애님 인스턴스를 가져옴. (애니메이션 블루프린트를 통해 가져오는것)
	Anim = Cast<USH_AnimInstance>(GetMesh()->GetAnimInstance());
	// 종료되면 알아서 공격 판별 변수를 false로 바꿔줌
	Anim->OnMontageEnded.AddDynamic(this, &ATestCharacter::OnTestAttackMontageEnded);

	// 노티파이들 델리게이트 바인딩
	Anim->OnTestAttackHitCheck.AddLambda([this]()->void {
	
		// 콤보 허용
		if (CurrentCombo < MaxCombo)
		{
			// 콤보 입력값을 허용함
			bNextComboInput = true;
		}
	});

	// 노티파이들 델리게이트 바인딩
	Anim->OnTestNextAttackCheck.AddLambda([this]()->void {

		// 다음 콤보 공격을 실행할지.
		if (bCanCombo)
		{
			// 콤보를 1로 올리고
			CurrentCombo++;
			// 다음 몽타주 섹션을 재생함.
			Anim->JumpToTestAttackMontageSection(CurrentCombo);

			// 초기화.
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
	// 이미 공격중이면
	if (bIsAttacking)
	{
		// 콤보 공격을 허용하는 상황인지
		if (bNextComboInput)
		{
			// 콤보 공격을 허용함.
			bCanCombo = true;
		}
		// 허용하지 않는지
		else
		{
			// pass. 무시
			return;
		}
	}
	// 공격중이지 않으면
	else
	{
		
		// 몽타주 첫 재생
		Anim->PlayTestAttackMontage();

		CurrentCombo++;
		bCanCombo = false;
		bNextComboInput = false;

		// 몽타주 재생 판별 변수 true로
		bIsAttacking = true;
	}

}

// 애니메이션이 끝나면
void ATestCharacter::OnTestAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	SH_LOG_S(Warning);
	// 공격 판별 변수 초기화
	bIsAttacking = false;

	// 각종 변수 초기화.
	CurrentCombo = 0;
	bNextComboInput = false;
	bCanCombo = false;

	Anim->SetCurrentSection();
}
