// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "GameFramework/GameStateBase.h"
#include "SH_GameState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASH_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASH_GameState();

public:
	int32 GetTotalGameScore() const;
	void AddGameScore();

	// ���� Ŭ���� ���� ����
	void SetGameCleared();
	bool IsGameCleared() const;

protected:

private:
	// ���� ���ھ�
	UPROPERTY(Transient)
	int32 TotalGameScore;

	// ���� Ŭ���� ����
	UPROPERTY(Transient)
	bool bGameCleared;
};
