// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_PlayerController.h"
#include "SungHoon/SH_HUDWidget.h"
#include "SungHoon/SH_PlayerState.h"
#include "SungHoon/SH_Character.h"
#include "SungHoon/SH_GameplayWidget.h"
#include "SungHoon/SH_GameplayResultWidget.h"
#include "SungHoon/SH_GameState.h"


ASH_PlayerController::ASH_PlayerController()
{
	// HUD 애셋 가져오기 생성 _C (게임 플레이할때 보이는 UI 화면 = HUD)
	static ConstructorHelpers::FClassFinder<USH_HUDWidget> SH_UI_HUD_C(TEXT(
		"/Game/SungHoon/UI/SH_UI_HUD.SH_UI_HUD_C"
	));
	// HUDWidgetClass에 애셋 지정
	if (SH_UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = SH_UI_HUD_C.Class;
	}

	// 메뉴 UI 블루프린트 위젯 클래스 정보 가져옴. _C
	static ConstructorHelpers::FClassFinder<USH_GameplayWidget> SH_UI_MENU_C(TEXT(
		"/Game/SungHoon/UI/SH_UI_Menu.SH_UI_Menu_C"));
	if (SH_UI_MENU_C.Succeeded())
	{
		// 위젯 클래스 등록
		MenuWidgetClass = SH_UI_MENU_C.Class;
	}

	// 최종 결과 UI 블루프린트 위젯 클래스 정보 가져옴. _C
	static ConstructorHelpers::FClassFinder<USH_GameplayResultWidget> SH_UI_RESULT_C(TEXT(
		"/Game/SungHoon/UI/SH_UI_Result.SH_UI_Result_C"));
	if (SH_UI_RESULT_C.Succeeded())
	{
		// 위젯 클래스 등록
		ResultWidgetClass = SH_UI_RESULT_C.Class;
	}
		
}

void ASH_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SH_LOG_S(Warning);
}

void ASH_PlayerController::OnPossess(APawn * aPawn)
{
	SH_LOG_S(Warning);
	Super::OnPossess(aPawn);
}

USH_HUDWidget* ASH_PlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

// NPC를 죽였을때
void ASH_PlayerController::NPCKill(ASH_Character * KilledNPC) const
{
	// NPC가 들고 있는 경험치를 PlayerState에 전달함.
	SHPlayerState->AddExp(KilledNPC->GetExp());
}


void ASH_PlayerController::AddGameScore() const
{
	SHPlayerState->AddGameScore();
}

void ASH_PlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		// 게임 모드. 커서 안보임
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		// UI 모드. 커서 보임
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ASH_PlayerController::ShowResultUI()
{
	// 현재 설정된 게임 스테이트를 가져옴
	auto SHGameState = Cast<ASH_GameState>(UGameplayStatics::GetGameState(this));
	SH_CHECK(SHGameState != nullptr);
	// 게임 스테이트를 결과 위젯에 연결함.
	ResultWidget->BindGameState(SHGameState);

	// 화면에 출력!
	ResultWidget->AddToViewport();
	// 입력값을 UI 전용으로
	ChangeInputMode(false);
}

void ASH_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 플레이를 누르면 마우스 클릭없이 바로 키보드 입력모드로 설정하는법.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// 실제 위젯 컴포넌트를 생성한다. 그 정보를 HUDWidget에 저장
	HUDWidget = CreateWidget<USH_HUDWidget>(this, HUDWidgetClass);
	// 생성한 위젯을 뷰포트에 추가
	HUDWidget->AddToViewport();

	// 실제 위젯 생성
	ResultWidget = CreateWidget<USH_GameplayResultWidget>(this, ResultWidgetClass);
	SH_CHECK(ResultWidget != nullptr);

	// PlayerState 가져옴. (PlayerState는 기본 Controller 상속된 멤버변수임)
	SHPlayerState = Cast<ASH_PlayerState>(PlayerState);
	SH_CHECK(SHPlayerState != nullptr);
	HUDWidget->BindPlayerState(SHPlayerState);
	
	// PlayerState의 OnPlayerStateChanged 델리게이트에 바인딩된 함수 호출!
	SHPlayerState->OnPlayerStateChanged.Broadcast();
}


void ASH_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("SH_GamePause"), EInputEvent::IE_Pressed,
		this, &ASH_PlayerController::OnGamePause);
}


void ASH_PlayerController::OnGamePause()
{
	// 메뉴 위젯 생성
	MenuWidget = CreateWidget<USH_GameplayWidget>(this, MenuWidgetClass);
	SH_CHECK(MenuWidget);

	// 뷰포트 출력
	MenuWidget->AddToViewport(3);
	
	// 일시정지
	SetPause(true);
	ChangeInputMode(false);
}