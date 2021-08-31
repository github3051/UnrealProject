// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_GameplayResultWidget.h"
#include "Components/TextBlock.h"
#include "SungHoon/SH_GameState.h"


void USH_GameplayResultWidget::BindGameState(ASH_GameState* GameState)
{
	SH_CHECK(GameState != nullptr);
	CurrentGameState = GameState;
}

void USH_GameplayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 현재 게임 스테이트가 유효한지
	SH_CHECK(CurrentGameState.IsValid());

	// 위젯에 있는 txtResult 텍스트블록을 가져옴
	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	SH_CHECK(Result != nullptr);
	// 결과 텍스트 박스 내용 설정
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Misson Complete!") : TEXT("Misson Failed!")));

	// 위젯에 있는 txtTotalScore 가져옴
	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	SH_CHECK(TotalScore != nullptr);
	// 전체 게임 스코어 상황을 텍스트 박스 내용으로 설정
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
}