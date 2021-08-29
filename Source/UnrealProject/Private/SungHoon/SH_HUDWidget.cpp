// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_HUDWidget.h"
#include "SungHoon/SH_CharacterStatComponent.h"
#include "SungHoon/SH_PlayerState.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USH_HUDWidget::BindCharacterStat(class USH_CharacterStatComponent* CharacterStat)
{
	// CharacterStat 정보가 제대로 넘어왔으면 통과
	SH_CHECK(CharacterStat!=nullptr);
	CurrentCharacterStat = CharacterStat;
	// 캐릭터 스텟에 있는 델리게이트를 여기서 델리게이트 함수로 바인딩
	CharacterStat->OnHPChanged.AddUObject(this, &USH_HUDWidget::UpdateCharacterStat);
}

void USH_HUDWidget::BindPlayerState(ASH_PlayerState * PlayerState)
{
	// 문제없으면 통과!
	SH_CHECK(PlayerState != nullptr);
	CurrentPlayerState = PlayerState;
	// 캐릭터 스테이트에 있는 델리게이트를 여기서 델리게이트 함수로 바인딩
	PlayerState->OnPlayerStateChanged.AddUObject(this, &USH_HUDWidget::UpdatePlayerState);
}

// UI 시스템이 준비 완료되면 자동 호출되는 함수.
void USH_HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SH_LOG_S(Warning);
	
	// 현재 위젯에서 pbHP라는 ProgressBar 정보를 가져옴.
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

// 캐릭터 스텟 관련
void USH_HUDWidget::UpdateCharacterStat()
{
	SH_CHECK(CurrentCharacterStat.IsValid());

	// CharacterStatComponent에서 HP 상태 가져와 등록
	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

// 캐릭터 상태 관련
void USH_HUDWidget::UpdatePlayerState()
{
	SH_CHECK(CurrentPlayerState.IsValid());
	
	// ExpBar
	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
	// PlayerState에서 이름 가져와 등록
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	// PlayerState에서 레벨 가져와 등록
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	// PlayerState에서 GameScore 가져와 등록
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
}
