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
	// HUD �ּ� �������� ���� _C (���� �÷����Ҷ� ���̴� UI ȭ�� = HUD)
	static ConstructorHelpers::FClassFinder<USH_HUDWidget> SH_UI_HUD_C(TEXT(
		"/Game/SungHoon/UI/SH_UI_HUD.SH_UI_HUD_C"
	));
	// HUDWidgetClass�� �ּ� ����
	if (SH_UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = SH_UI_HUD_C.Class;
	}

	// �޴� UI �������Ʈ ���� Ŭ���� ���� ������. _C
	static ConstructorHelpers::FClassFinder<USH_GameplayWidget> SH_UI_MENU_C(TEXT(
		"/Game/SungHoon/UI/SH_UI_Menu.SH_UI_Menu_C"));
	if (SH_UI_MENU_C.Succeeded())
	{
		// ���� Ŭ���� ���
		MenuWidgetClass = SH_UI_MENU_C.Class;
	}

	// ���� ��� UI �������Ʈ ���� Ŭ���� ���� ������. _C
	static ConstructorHelpers::FClassFinder<USH_GameplayResultWidget> SH_UI_RESULT_C(TEXT(
		"/Game/SungHoon/UI/SH_UI_Result.SH_UI_Result_C"));
	if (SH_UI_RESULT_C.Succeeded())
	{
		// ���� Ŭ���� ���
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

// NPC�� �׿�����
void ASH_PlayerController::NPCKill(ASH_Character * KilledNPC) const
{
	// NPC�� ��� �ִ� ����ġ�� PlayerState�� ������.
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
		// ���� ���. Ŀ�� �Ⱥ���
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		// UI ���. Ŀ�� ����
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ASH_PlayerController::ShowResultUI()
{
	// ���� ������ ���� ������Ʈ�� ������
	auto SHGameState = Cast<ASH_GameState>(UGameplayStatics::GetGameState(this));
	SH_CHECK(SHGameState != nullptr);
	// ���� ������Ʈ�� ��� ������ ������.
	ResultWidget->BindGameState(SHGameState);

	// ȭ�鿡 ���!
	ResultWidget->AddToViewport();
	// �Է°��� UI ��������
	ChangeInputMode(false);
}

void ASH_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̸� ������ ���콺 Ŭ������ �ٷ� Ű���� �Է¸��� �����ϴ¹�.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// ���� ���� ������Ʈ�� �����Ѵ�. �� ������ HUDWidget�� ����
	HUDWidget = CreateWidget<USH_HUDWidget>(this, HUDWidgetClass);
	// ������ ������ ����Ʈ�� �߰�
	HUDWidget->AddToViewport();

	// ���� ���� ����
	ResultWidget = CreateWidget<USH_GameplayResultWidget>(this, ResultWidgetClass);
	SH_CHECK(ResultWidget != nullptr);

	// PlayerState ������. (PlayerState�� �⺻ Controller ��ӵ� ���������)
	SHPlayerState = Cast<ASH_PlayerState>(PlayerState);
	SH_CHECK(SHPlayerState != nullptr);
	HUDWidget->BindPlayerState(SHPlayerState);
	
	// PlayerState�� OnPlayerStateChanged ��������Ʈ�� ���ε��� �Լ� ȣ��!
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
	// �޴� ���� ����
	MenuWidget = CreateWidget<USH_GameplayWidget>(this, MenuWidgetClass);
	SH_CHECK(MenuWidget);

	// ����Ʈ ���
	MenuWidget->AddToViewport(3);
	
	// �Ͻ�����
	SetPause(true);
	ChangeInputMode(false);
}