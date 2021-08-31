// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_UIPlayerController.h"
#include "Blueprint/UserWidget.h"


void ASH_UIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UIWidgetClass�� �����Ǿ� ������ ���
	SH_CHECK(UIWidgetClass != nullptr);

	// �� Ŭ������ �������� ������ ������
	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	SH_CHECK(UIWidgetInstance != nullptr);

	// ȭ�鿡 ���
	UIWidgetInstance->AddToViewport();

	// �Է��� UI���� ���޵ǵ���
	FInputModeUIOnly Mode;
	// �Է��� Ư�� �������� ���޵ǵ���
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	// ���콺 Ŀ�� ���̴� �Ӽ� true
	bShowMouseCursor = true;
}