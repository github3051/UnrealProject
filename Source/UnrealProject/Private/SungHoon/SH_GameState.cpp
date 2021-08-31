// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_GameState.h"



ASH_GameState::ASH_GameState()
{
	TotalGameScore = 0;
	bGameCleared = false;
}

int32 ASH_GameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void ASH_GameState::AddGameScore()
{
	TotalGameScore++;
}

// 게임 클리어 변수 조정 setter
void ASH_GameState::SetGameCleared()
{
	bGameCleared = true;
}

// getter
bool ASH_GameState::IsGameCleared() const
{
	return bGameCleared;
}