// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_Character.h"
#include "SungHoon/SH_AnimInstance.h" // Added SH_AnimInstance, for Attack Montage
#include "SungHoon/SH_Weapon.h"
#include "SungHoon/SH_CharacterStatComponent.h" // 전방선언한거 구체화
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
// UnrealProjectSetting 모듈에 있는거 추가
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
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); // A_BP 모드 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> SH_CARDBOARD_ANIM(TEXT(
		"/Game/SungHoon/Animation/SH_WarriorAnimBlueprint.SH_WarriorAnimBlueprint_C"
	));
	if (SH_CARDBOARD_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SH_CARDBOARD_ANIM.Class);
	}

	// Set default 컨트롤 모드 설정. DIABLO 방식
	// SetControlMode(EControlMode::GTA); // -> Possessedby에서 초기화

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f; // 회전 속도

	// for jump speed
	GetCharacterMovement()->JumpZVelocity = 450.0f;

	// for attack's montage
	IsAttacking = false;

	// for attack combo
	MaxCombo = 4;
	AttackEndComboState(); // 콤보가 끝난 상태로 초기화

	// Set Collision channel
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SH_Character"));

	// for Debug Drawing by Capsule
	AttackRange = 80.0f; // 공격 범위
	AttackRadius = 50.0f; // 반지름

	// CharacterStat 컴포넌트 생성.
	CharacterStat = CreateDefaultSubobject<USH_CharacterStatComponent>(TEXT("SH_CHARACTERSTAT"));

	/*--------------------------------
					UI
	---------------------------------*/
	// HPBar UI 위젯
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SH_HPBARWIDGET"));
	// 메시에 부착
	HPBarWidget->SetupAttachment(GetMesh());
	// 화면상의 최초 위치 조정
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	// 항상 플레이어의 화면을 향해 보도록 Screen 모드 설정.
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	// 애셋 가져오기. 형태는 UUserWidget이다. 클래스형태. 경로에 _C 넣어주기.
	static ConstructorHelpers::FClassFinder<UUserWidget> SH_UI_HUD(TEXT(
		"/Game/SungHoon/UI/SH_HPBar.SH_HPBar_C"));
	if (SH_UI_HUD.Succeeded())
	{
		// 애셋을 설정함. 위젯은 클래스임.
		HPBarWidget->SetWidgetClass(SH_UI_HUD.Class);
		// 위젯의 크기 지정
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	/*--------------------------------
					AI
	---------------------------------*/
	// AI 컨트롤러 설정으로 SH_AIController 클래스로 셋팅
	AIControllerClass = ASH_AIController::StaticClass();
	// 자동 AI 설정으로 월드에 미리 배치되거나 새로 생성되는 모든것들을 의미함.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	// CardBoard 에셋 번호
	AssetIndex = 3;
	// 현재 캐릭터를 숨김
	SetActorHiddenInGame(true);
	// UI도 숨김
	HPBarWidget->SetHiddenInGame(true);
	// 데미지 프레임워크 끔
	SetCanBeDamaged(false);

	// 죽음 지연 시간 5초
	DeadTimer = 5.0f;
}

// Called when the game starts or when spawned
void ASH_Character::BeginPlay()
{
	Super::BeginPlay();

	/*
		언리얼 4.21 이후로는 위젯의 초기화 시점이 PostInitializeComponents에서 BeginPlay로 변경됨.
		올바르게 동작하려면 위젯들은 BeginPlay에 추가하도록 하자.
	*/

	// 캐릭터 위젯 델리게이트 연결. HPBar 위젯에 연결되어있는 (부모) 위젯 클래스 정보를 가져옴. (일종의 애님인스턴스 가져오기)
	auto CharacterWidget = Cast<USH_CharacterWidget>(HPBarWidget->GetUserWidgetObject());
	// 유효하다면
	if (CharacterWidget != nullptr)
	{
		// 캐릭터가 들고있는 CharacterStat 정보를 넘김.
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

	// 플레이어 컨트롤러 값을 가져옴
	bIsPlayer = IsPlayerControlled();


	// 플레이어라면
	if (bIsPlayer)
	{
		// 플레이어 컨트롤러 가져오기
		SHPlayerController = Cast<ASH_PlayerController>(GetController());
		// 컨트롤러 값을 제대로 캐스팅해서 가져왔다면 통과
		SH_CHECK(SHPlayerController != nullptr);
	}
	// 플레이어가 아니라면 = AI라면
	else
	{
		// AI 컨트롤러 가져오기
		SHAIController = Cast<ASH_AIController>(GetController());
		// 컨트롤러 값을 제대로 캐스팅해서 가져왔다면 통과
		SH_CHECK(SHAIController != nullptr);
	}

	// USH_CharacterSetting에 대한 CDO 읽어옴
	auto DefaultSetting = GetDefault<USH_CharacterSetting>();

	if (bIsPlayer)
	{
		AssetIndex = 3;
	}
	else
	{
		// ini에서 읽어온 개수. 배열의 0번 인덱스부터 시작하므로 -1 해줌.
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	// 애셋 지정
	{
		// 애셋 경로를 하나 읽어옴
		CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
		// 현재 프로젝트 설정의 게임 인스턴스를 가져옴. 그걸 캐스팅 (헤더 추가 필요)
		auto SHGameInstance = Cast<USH_GameInstance>(GetGameInstance());
		// 성공적이라면
		if (SHGameInstance != nullptr)
		{
			// 비동기 방식으로 게임 실행중에도 경로에 있는 애셋을 요청함. 그리고 델리게이트 바인딩된 함수 실행함.
			AssetStreamingHandle = SHGameInstance->StreamableManager.RequestAsyncLoad(
				CharacterAssetToLoad, FStreamableDelegate::CreateUObject(
					this, &ASH_Character::OnAssetLoadCompleted));
		}

		// 현재 캐릭터 state를 LOADING으로 변경
		SetCharacterState(ESH_CharacterState::LOADING);
	}
}

// 캐릭터의 State를 설정
void ASH_Character::SetCharacterState(ESH_CharacterState NewState)
{
	// 현재 state가 새로운 state와 다를때만 통과
	SH_CHECK(CurrentState != NewState);
	CurrentState = NewState;

	// 현재 상태에 따른 분류
	switch (CurrentState)
	{
	case ESH_CharacterState::PREINIT:
		break;
	case ESH_CharacterState::LOADING:
	{
		// 플레이어 라면
		if (bIsPlayer)
		{
			// 플레이어의 조작을 막음
			DisableInput(SHPlayerController);

			// CharacterStat 정보를 HUD에 연결함.
			SHPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			// 플레이어 스테이트 가져옴
			auto SHPlayerState = Cast<ASH_PlayerState>(GetPlayerState());
			SH_CHECK(SHPlayerState != nullptr);
			// 플레이어의 레벨을 새롭게 레벨 설정.
			CharacterStat->SetNewLevel(SHPlayerState->GetCharacterLevel());
		}
		// AI라면
		else
		{
			// 게임 모드를 가져옴. 게임 실행중일때 게임 모드를 가져오는 함수.
			auto SHGameMode = Cast<ASH_GameModeBase>(GetWorld()->GetAuthGameMode());
			SH_CHECK(SHGameMode != nullptr);
			// 게임 모드에서 Score값을 가져와 0.8을 곱한 값을 AI 레벨로 설정
			int32 TargetLevel = FMath::CeilToInt(((float)SHGameMode->GetScore()* 0.8f));
			// TargetLevle을 1~20 사이의 정수로 확정지음. 
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			SH_LOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			// 캐릭터 스텟으로가 해당 레벨로 레벨 설정해줌.
			CharacterStat->SetNewLevel(FinalLevel);
		}

		// 엑터를 숨김
		SetActorHiddenInGame(true);
		// UI 숨김
		HPBarWidget->SetHiddenInGame(true);
		// 데미지 프레임워크 끔
		SetCanBeDamaged(false);
		break;
	}
	case ESH_CharacterState::READY:
	{
		// 액터 보이기
		SetActorHiddenInGame(false);
		// UI 보이기
		HPBarWidget->SetHiddenInGame(false);
		// 데미지 프레임워크 작동
		SetCanBeDamaged(true);

		// 캐릭터 스텟의 죽었을때의 델리게이트 함수를 바인딩함.
		CharacterStat->OnHPIsZero.AddLambda([this]()->void {
			// 죽음 상태로 바꿈. 자기자신 호출인셈.
			SetCharacterState(ESH_CharacterState::DEAD);
		});

		// HPBarWidget 클래스 정보를 가져옴
		auto CharacterWidget = Cast<USH_CharacterWidget>(HPBarWidget->GetUserWidgetObject());
		// 제대로 위젯정보를 가져왔다면 통과
		SH_CHECK(CharacterWidget != nullptr);
		// 캐릭터 위젯 정보를 Character Stat과 연결지음
		CharacterWidget->BindCharacterStat(CharacterStat);

		// 플레이어라면
		if (bIsPlayer)
		{
			// GTA모드로 설정
			SetControlMode(EControlMode::GTA);
			// 움직임 속도 조절
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			// 조작 허용
			EnableInput(SHPlayerController);
		}
		// AI라면
		else
		{
			// NPC 모드로 설정
			SetControlMode(EControlMode::NPC);
			// 움직임 속도 조절
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			// 비헤이비어트리 구동
			SHAIController->RunAI();
		}

		break;
	}
	case ESH_CharacterState::DEAD:
	{
		// 해당 캐릭터의 충돌처리를 끔
		SetActorEnableCollision(false);
		// 혹시 메시가 꺼져있다면 킴
		GetMesh()->SetHiddenInGame(false);
		// HP UI 숨김
		HPBarWidget->SetHiddenInGame(true);
		// 죽은 애니메이션 작동
		SHAnim->SetDeadAnim();
		// 데미지 프레임워크 끔
		SetCanBeDamaged(false);

		// 플레이어라면
		if (bIsPlayer)
		{
			// 조작 막음
			DisableInput(SHPlayerController);
		}
		// AI라면
		else
		{
			// 비헤이비어 트리 정지
			SHAIController->StopAI();
		}

		// 타이머 설정. DeadTimer 이후에 람다함수 실행됨.
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {

			// 플레이어라면
			if (bIsPlayer)
			{
				// 레벨을 아예 다시 시작해라
				SHPlayerController->RestartLevel();
			}
			// AI라면
			else
			{
				// 오브젝트 파괴
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

	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		// 회전 속도
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
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	// for Attack
	PlayerInputComponent->BindAction(TEXT("SH_Attack"), EInputEvent::IE_Pressed, this, &ASH_Character::Attack);
}

void ASH_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 델리게이트 사용을 위해 애님인스턴스를 임시로 가져온다.
	SHAnim = Cast<USH_AnimInstance>(GetMesh()->GetAnimInstance());

	// 거짓이면 매크로 출력
	SH_CHECK(SHAnim != nullptr);

	// 애님인스턴스에 있는 함수를 캐릭터 클래스에 dynamic 델리게이트 등록.
	SHAnim->OnMontageEnded.AddDynamic(this, &ASH_Character::OnAttackMontageEnded);

	// 몽타주 애니메이션에서 NextAttackCheck 노티파이가 실행되면 호출하는 델리게이트 함수(이벤트)
	// 애님 인스턴스에서 OnAttackCheck.BroadCast를 하면 람다함수가 호출됨.
	// Multicast 델리게이트
	SHAnim->OnNextAttackCheck.AddLambda([this]()->void {

		SH_LOG(Warning, TEXT("OnNextAttackCheck Lambda Function Called! CurrentCombo : %d"), CurrentCombo);

		CanNextCombo = false;
		// 콤보공격이 적절한 타이밍에 들어왔다면
		if (IsComboInputOn)
		{
			AttackStartComboState();
			SHAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	// 공격을 위한 델리게이트 등록. 몽타주 애니메이션의 AttackHitCheck 노티파이 실행시 자동 호출
	// Muticast 델리게이트
	SHAnim->OnAttackHitCheck.AddUObject(this, &ASH_Character::AttackCheck);

	// 죽는것에 대한 이벤트를 등록하기 위한 델리게이트 등록!
	CharacterStat->OnHPIsZero.AddLambda([this]()->void {

		SH_LOG(Error, TEXT("OnHPIsZero Delegate function call!"));
		// 죽음 설정
		SHAnim->SetDeadAnim();
		// 충돌처리 꺼줌
		SetActorEnableCollision(false);
	});


}

// 경험치 획득량 반환
int32 ASH_Character::GetExp() const
{
	return CharacterStat->GetDropExp();
}

// 최종 공격 범위 함수
float ASH_Character::GetFinalAttackRange() const
{
	// 무기가 있으면 무기 공격 범위, 아니면 기본 캐릭터 공격 범위를 설정.
	return (CurrentWeapon != nullptr) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

// 최종 공격 데미지 값 계산 함수
float ASH_Character::GetFinalAttackDamage() const
{
	// 무기가 있을때와 없을때를 나눠서 계산함.
	float AttackDamage = (CurrentWeapon != nullptr) ?
		(CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : CharacterStat->GetAttack();
	// 무기가 있을때와 없을때
	float AttackModifer = (CurrentWeapon != nullptr) ? CurrentWeapon->GetAttackModifier() : 1.0f;

	// 실제 공격력은 무기 공격력 * 추가 데미지 연산값
	return AttackDamage * AttackModifer;
}

float ASH_Character::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// 최종적으로 넘어오는 데미지값.
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	SH_LOG(Warning, TEXT("Actor : %s took damage : %f"), *GetName(), FinalDamage);

	// 최종 데미지를 스탯에서 연산해줌.
	CharacterStat->SetDamage(FinalDamage);

	// 현재 상태가 죽음이라면
	if (CurrentState == ESH_CharacterState::DEAD)
	{
		// 대상자가 플레이어라면
		if (EventInstigator->IsPlayerController())
		{
			// 플레이어의 컨트롤러를 가져옴.
			auto Instigator_PlayerController = Cast<ASH_PlayerController>(EventInstigator);
			SH_CHECK(Instigator_PlayerController != nullptr, 0.0f);
			// NPC 죽음을 처리. 경험치 연산.
			Instigator_PlayerController->NPCKill(this);
		}
	}
	return FinalDamage;
}

// 빙의 됐을때 호출. 컨트롤러와 폰이 연결됐을때.
void ASH_Character::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	// 빙의 되고 로그 찍어보기
	SH_LOG_S(Warning);

	// 플레이어라면
	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::GTA);
		// 최대 이동 속도
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	// AI 라면
	else
	{
		// NPC 설정
		SetControlMode(EControlMode::NPC);
		// 최대 이동 속도
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

bool ASH_Character::CanSetWeapon()
{
	// 무기가 없다면 true
	//return (CurrentWeapon == nullptr);

	// 항상 교체할 수 있게 수정한다. chapter 14
	return true;
}

void ASH_Character::SetWeapon(ASH_Weapon* NewWeapon)
{
	// 무기 애셋정보가 잘 들어왔으면 통과
	SH_CHECK(NewWeapon != nullptr);

	// 무기가 이미 있다면 해제하고 파괴함.
	if (CurrentWeapon != nullptr)
	{
		// 액터와 연결된 부분을 해제함. Attachment 함수로 붙은거
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy(); // 무기를 파괴함.
		CurrentWeapon = nullptr; // 포인터값 초기화 했음.
	}

	// 실제 스켈레탈 메시의 소켓명과 정확히 일치해야함.
	FName WeaponSocket(TEXT("hand_rSocket")); // string 변수
	// 메시에 해당하는 소켓명이 존재한다면
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		// 무기 액터를 캐릭터 매시에 붙임.
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		// 무기 액터를 이 캐릭터 클래스에 상속시킴.
		NewWeapon->SetOwner(this);
		// 붙인 무기 액터에 대한 클래스에 대한 포인터를 저장
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

void ASH_Character::Attack()
{
	// 마우스 왼쪽 클릭할때마다 실행됨.
	SH_LOG_S(Error);

	// 공격중이면 반환
	if (IsAttacking)
	{
		SH_CHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));

		// 다음 콤보를 진행할 수 있다면
		if (CanNextCombo)
		{
			// 콤보 연계를 true로 해준다.
			IsComboInputOn = true;
		}
	}
	else
	{
		// CurrentCombo가 제대로 초기화 됐는지 체크
		SH_CHECK(CurrentCombo == 0);
		AttackStartComboState(); // 최초 콤보 시작.
		// 애님 인스턴스의 함수를 캐릭터에서 호출함.
		SHAnim->PlayAttackMontage(); // 공격 몽타주를 실행.
		SHAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true; // 공격중
	}
}

// 다이나믹 델리게이트 함수. 공격 몽타주가 끝나면 실행해주길 바라는 함수.
void ASH_Character::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	SH_CHECK(IsAttacking);
	SH_CHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	// 공격 애니메이션이 끝나면 관련된 모든 함수를 호출함.
	OnAttackEnd.Broadcast();
}

void ASH_Character::AttackStartComboState()
{
	// 다음 단계로 가능
	CanNextCombo = true;
	// 입력값은 초기화
	IsComboInputOn = false;

	// bool(0 <= CurrentCombo <= MaxCombo - 1 (=3))
	SH_CHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));

	// 1 <= CurrentCombo + 1 <= MaxCombo(=4)
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ASH_Character::AttackEndComboState()
{
	// 전부 초기화
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ASH_Character::AttackCheck()
{
	// 공격 범위를 가져옴
	float FinalAttackRange = GetFinalAttackRange();


	// 충돌처리 결과를 받을 구조체 변수 선언
	FHitResult HitResult;

	// 탐색 방법을 위한 설정 파라미터
	FCollisionQueryParams Params(NAME_None, false, this);

	// 충돌체크 여부 bool 변수
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius), //구체
		Params // 파라미터. 나 자신은 탐색에서 제외.
	);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	// 캡슐의 중앙값
	FVector Center = GetActorLocation() + TraceVec * 0.5f; // 절반
	// 캡슐의 실제 높이
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
	// 캡슐의 Z축의 방향 설정. 쿼터니언으로 바꿈
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	// 충돌이 있으면 초록, 없으면 빨강
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	// 화면에 보이는 시간
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
		// 충돌한 대상이 액터라면, GC에 의해 삭제되지 않았다면(사용중이지 않아서)
		if (HitResult.Actor.IsValid())
		{
			SH_LOG(Error, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}

// BeginPlay에서 등록된 델리게이트 함수. 로딩한 메시 애셋을 등록하는 함수
void ASH_Character::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());

	// 포인터 리셋 시킴. Shared 포인터는 한번 사용했으면 이제 초기화 해줘야함.
	AssetStreamingHandle.Reset();
	// 만약 애셋 정보를 가져왔으면 통과.
	SH_CHECK(AssetLoaded != nullptr);
	// 메시에 등록함. 원래 생성자에
	GetMesh()->SetSkeletalMesh(AssetLoaded);
	// 폰의 STATE를 READY 상태로 바꿈.
	SetCharacterState(ESH_CharacterState::READY);
}
