// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_GameplayWidget.h"
#include "Components/Button.h"
#include "SungHoon/SH_PlayerController.h"


void USH_GameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 각 버튼들 객체 가져와서 캐스팅한후, 바인딩함.
	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (ResumeButton != nullptr)
	{
		ResumeButton->OnClicked.AddDynamic(this, &USH_GameplayWidget::OnResumClicked);
	}
	
	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (ReturnToTitleButton != nullptr)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &USH_GameplayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (RetryGameButton != nullptr)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &USH_GameplayWidget::OnRetryGameClicked);
	}
}


void USH_GameplayWidget::OnResumClicked()
{
	// 현재 플레이어의 컨트롤러를 가져옴
	auto SHPlayerController = Cast<ASH_PlayerController>(GetOwningPlayer());
	SH_CHECK(SHPlayerController != nullptr);

	// 현재 뷰포트에 띄워진 자신 UI를 제거
	RemoveFromParent();
	// 게임전용 Input로 재설정
	SHPlayerController->ChangeInputMode(true); 
	// 일시정지 풂.
	SHPlayerController->SetPause(false);
}


void USH_GameplayWidget::OnReturnToTitleClicked()
{
	// Title 레벨을 엶.
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("SH_Title"));
}

// 재시작 버튼
void USH_GameplayWidget::OnRetryGameClicked()
{
	auto SHPlayerController = Cast<ASH_PlayerController>(GetOwningPlayer());
	SH_CHECK(SHPlayerController != nullptr);
	// 현재 플레이어 컨트롤러를 가지고 재시작.
	SHPlayerController->RestartLevel();
}