// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_GameState.h"



ASH_GameState::ASH_GameState()
{
	TotalGameScore = 0;
}

int32 ASH_GameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void ASH_GameState::AddGameScore()
{
	TotalGameScore++;
}
