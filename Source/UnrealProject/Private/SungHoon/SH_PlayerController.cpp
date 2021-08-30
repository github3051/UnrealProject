// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_PlayerController.h"
#include "SungHoon/SH_HUDWidget.h"
#include "SungHoon/SH_PlayerState.h"
#include "SungHoon/SH_Character.h"



ASH_PlayerController::ASH_PlayerController()
{
	// �ּ� �������� ����
	static ConstructorHelpers::FClassFinder<USH_HUDWidget> SH_UI_HUD_C(TEXT(
		"/Game/SungHoon/UI/SH_UI_HUD.SH_UI_HUD_C"
	));
	// HUDWidgetClass�� �ּ� ����
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

	// PlayerState ������. (PlayerState�� �⺻ Controller ��ӵ� ���������)
	SHPlayerState = Cast<ASH_PlayerState>(PlayerState);
	SH_CHECK(SHPlayerState != nullptr);
	HUDWidget->BindPlayerState(SHPlayerState);
	
	// PlayerState�� OnPlayerStateChanged ��������Ʈ�� ���ε��� �Լ� ȣ��!
	SHPlayerState->OnPlayerStateChanged.Broadcast();
}
