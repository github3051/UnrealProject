// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_HUDWidget.h"
#include "SungHoon/SH_CharacterStatComponent.h"
#include "SungHoon/SH_PlayerState.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USH_HUDWidget::BindCharacterStat(class USH_CharacterStatComponent* CharacterStat)
{
	// CharacterStat ������ ����� �Ѿ������ ���
	SH_CHECK(CharacterStat!=nullptr);
	CurrentCharacterStat = CharacterStat;
	// ĳ���� ���ݿ� �ִ� ��������Ʈ�� ���⼭ ��������Ʈ �Լ��� ���ε�
	CharacterStat->OnHPChanged.AddUObject(this, &USH_HUDWidget::UpdateCharacterStat);
}

void USH_HUDWidget::BindPlayerState(ASH_PlayerState * PlayerState)
{
	// ���������� ���!
	SH_CHECK(PlayerState != nullptr);
	CurrentPlayerState = PlayerState;
	// ĳ���� ������Ʈ�� �ִ� ��������Ʈ�� ���⼭ ��������Ʈ �Լ��� ���ε�
	PlayerState->OnPlayerStateChanged.AddUObject(this, &USH_HUDWidget::UpdatePlayerState);
}

// UI �ý����� �غ� �Ϸ�Ǹ� �ڵ� ȣ��Ǵ� �Լ�.
void USH_HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SH_LOG_S(Warning);
	
	// ���� �������� pbHP��� ProgressBar ������ ������.
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	SH_CHECK(HPBar != nullptr);
	
	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExP")));
	SH_CHECK(ExpBar != nullptr);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	SH_CHECK(PlayerName != nullptr);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	SH_CHECK(PlayerLevel != nullptr);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	SH_CHECK(CurrentScore != nullptr);

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	SH_CHECK(HighScore != nullptr);
}

// ĳ���� ���� ����
void USH_HUDWidget::UpdateCharacterStat()
{
	SH_CHECK(CurrentCharacterStat.IsValid());

	// CharacterStatComponent���� HP ���� ������ ���
	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

// ĳ���� ���� ����
void USH_HUDWidget::UpdatePlayerState()
{
	SH_CHECK(CurrentPlayerState.IsValid());
	
	// ExpBar
	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
	// PlayerState���� �̸� ������ ���
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	// PlayerState���� ���� ������ ���
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	// PlayerState���� GameScore ������ ���
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
	// HigtScore�� ����
	HighScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameHighScore())));

}