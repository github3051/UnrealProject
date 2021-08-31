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

	// ���� ���� ������Ʈ�� ��ȿ����
	SH_CHECK(CurrentGameState.IsValid());

	// ������ �ִ� txtResult �ؽ�Ʈ����� ������
	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	SH_CHECK(Result != nullptr);
	// ��� �ؽ�Ʈ �ڽ� ���� ����
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Misson Complete!") : TEXT("Misson Failed!")));

	// ������ �ִ� txtTotalScore ������
	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	SH_CHECK(TotalScore != nullptr);
	// ��ü ���� ���ھ� ��Ȳ�� �ؽ�Ʈ �ڽ� �������� ����
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
}