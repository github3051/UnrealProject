// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_UIPlayerController.h"
#include "Blueprint/UserWidget.h"


void ASH_UIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UIWidgetClass가 지정되어 있으면 통과
	SH_CHECK(UIWidgetClass != nullptr);

	// 이 클래스를 기준으로 위젯을 생성함
	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	SH_CHECK(UIWidgetInstance != nullptr);

	// 화면에 띄움
	UIWidgetInstance->AddToViewport();

	// 입력이 UI에만 전달되도록
	FInputModeUIOnly Mode;
	// 입력이 특정 위젯에만 전달되도록
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	// 마우스 커서 보이는 속성 true
	bShowMouseCursor = true;
}