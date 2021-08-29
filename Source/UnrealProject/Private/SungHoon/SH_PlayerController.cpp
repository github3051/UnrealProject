// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_PlayerController.h"
#include "SungHoon/SH_HUDWidget.h"
#include "SungHoon/SH_PlayerState.h"
#include "SungHoon/SH_Character.h"



ASH_PlayerController::ASH_PlayerController()
{
	// 애셋 가져오기 생성
	static ConstructorHelpers::FClassFinder<USH_HUDWidget> SH_UI_HUD_C(TEXT(
		"/Game/SungHoon/UI/SH_UI_HUD.SH_UI_HUD_C"
	));
	// HUDWidgetClass에 애셋 지정
	if (SH_UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = SH_UI_HUD_C.Class;
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
	SHPlayerState->AddScore();
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

	// PlayerState 가져옴. (PlayerState는 기본 Controller 상속된 멤버변수임)
	SHPlayerState = Cast<ASH_PlayerState>(PlayerState);
	SH_CHECK(SHPlayerState != nullptr);
	HUDWidget->BindPlayerState(SHPlayerState);
	
	// PlayerState의 OnPlayerStateChanged 델리게이트에 바인딩된 함수 호출!
	SHPlayerState->OnPlayerStateChanged.Broadcast();
}
