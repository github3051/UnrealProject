// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_Character.h"

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
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); // A_BP 모드 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> SH_CARDBOARD_ANIM(TEXT(
		"/Game/SungHoon/Animation/SH_WarriorAnimBlueprint.SH_WarriorAnimBlueprint_C"
	));
	if (SH_CARDBOARD_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SH_CARDBOARD_ANIM.Class);
	}

	// Set default 컨트롤 모드 설정. DIABLO 방식
	SetControlMode(EControlMode::GTA);

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f; // 회전 속도

	// for jump speed
	GetCharacterMovement()->JumpZVelocity = 450.0f;
}

// Called when the game starts or when spawned
void ASH_Character::BeginPlay()
{
	Super::BeginPlay();

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
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator); // 일자 정면을 보도록.
		ArmLengthTo = 600.0f; // distance
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
		break;

	case EControlMode::DIABLO:
		SH_LOG(Error, TEXT("DIABLO Mode"));

		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f,0.0f,0.0f)); // 아래로 꺾음.
		ArmLengthTo = 900.0f; // distance
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f); // 카메라 각도
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bInheritRoll = false;

		// 카메라와 벽의 충돌을 체크해줌. 벽이 있으면 알아서 줌인되는걸 끔.
		SpringArm->bDoCollisionTest = false;

		// Pawn 설정의 빙글빙글 회전 막음. (기존 방식 막음)
		// 대신 밑에서 bUseControllerDesiredRotation를 켜서 해결함. 둘다 켜져있으면 안된데!
		bUseControllerRotationYaw = false;

		// CharacterMovement에 있는 기능을 이용하자.
		// 카메라가 움직이는 방향으로 메시를 움직이게 하는 기능임. false. 고정된 시점이므로.
		GetCharacterMovement()->bOrientRotationToMovement = false;

		// 45도씩 끊겨서 회전하는것을 해결하기 위해, 부드럽게 회전하는 기능. 켜주자.
		GetCharacterMovement()->bUseControllerDesiredRotation = true; // 부드러운 회전을 위해

		// 캐릭터 메시의 회전속도 지정 bUseControllerDesiredRotation의 전환 속도를 의미.
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;

	default:
		break;
	}
}

// Called every frame
void ASH_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 매 프레임마다 카메라 손잡이 거리 계산
	// InterpTo(현재 값, 최종 목표값, 시간, 보간속도)
	 SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		break;

	case EControlMode::DIABLO:
		/*
			DIABLO 방식은 입력값을 받고 바로 움직이고 회전하는게 아니라,
			해당하는 회전과 움직임값을 모두 처리한 뒤에, 최종적으로 Tick에서
			결과값을 이용해서 이동하고 회전하도록 만듦. 그래서 실제 움직임이
			Tick함수에 있다. GTA는 아님. GTA는 입력함수에 있음.
		*/

		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));

		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			// rotation (메시가 돌아감) 
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			// move (실제 이동)
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
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed, this, &ACharacter::Jump);
}

// for forward, back move
void ASH_Character::UpDown(const float NewAxisValue)
{
	// AddMovementInput(GetActorForwardVector(), NewAxisValue);
	// AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//AddMovementInput(GetActorForwardVector(), NewAxisValue); // 메시가 바라보는 방향 기준으로 움직임.

		// 현재 컨트롤러의 회전값을 가져와서 단위벡터로 방향을 구해옴. 그 방향으로 이동함.
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
		//AddMovementInput(GetActorRightVector(), NewAxisValue); // 메시의 오른쪽 방향을 기준으로 움직임.

		// 현재 컨트롤러의 회전값을 가져와서 단위벡터로 방향을 구해옴. 그 방향으로 이동함.
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

// Pitch (위아래)
void ASH_Character::LookUp(const float NewAxisValue)
{
	// 위아래 회전은 Y축 기준 회전(오른 축)
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

// Yaw (좌우 빙글빙글)
void ASH_Character::Turn(const float NewAxisValue)
{
	// 옆으로 회은 Z축 기준 회전 (위쪽 축)
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
		// 현재 모드가 GTA라면
	case EControlMode::GTA:
		// for DIABLO
		/*
			아래 컨트롤러의 회전값을 수정해주는 이유가 뭘까?
		*/
		// Controller의 rotation값을 현재 액터의 Rotation값으로 설정.
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);;
		break;

		// 현재 모드가 DIABLO라면
	case EControlMode::DIABLO:
		// for GTA
		/*
			DIABLO는 SpringArm을 기준으로 회전하기 때문.
			이걸 다시 컨트롤러의 회전값으로 수정해줄 필요가 있음.
			이걸 안해주면 GTA로 변환할때 항상 Default 카메라 설정으로,
			정면샷만 나옴.
			즉, 카메라의 높이 수준을 그대로 가져온다면, 좀더 자연스러워지는거지.
		*/
		// 현재 SpringArm의 회전값을 ControlRotation 값으로 설정.
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::GTA);;
		break;

	default:
		break;
	}
}