// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_GameplayWidget.h"
#include "Components/Button.h"
#include "SungHoon/SH_PlayerController.h"


void USH_GameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// �� ��ư�� ��ü �����ͼ� ĳ��������, ���ε���.
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
	// ���� �÷��̾��� ��Ʈ�ѷ��� ������
	auto SHPlayerController = Cast<ASH_PlayerController>(GetOwningPlayer());
	SH_CHECK(SHPlayerController != nullptr);

	// ���� ����Ʈ�� ����� �ڽ� UI�� ����
	RemoveFromParent();
	// �������� Input�� �缳��
	SHPlayerController->ChangeInputMode(true); 
	// �Ͻ����� ǯ.
	SHPlayerController->SetPause(false);
}


void USH_GameplayWidget::OnReturnToTitleClicked()
{
	// Title ������ ��.
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("SH_Title"));
}

// ����� ��ư
void USH_GameplayWidget::OnRetryGameClicked()
{
	auto SHPlayerController = Cast<ASH_PlayerController>(GetOwningPlayer());
	SH_CHECK(SHPlayerController != nullptr);
	// ���� �÷��̾� ��Ʈ�ѷ��� ������ �����.
	SHPlayerController->RestartLevel();
}