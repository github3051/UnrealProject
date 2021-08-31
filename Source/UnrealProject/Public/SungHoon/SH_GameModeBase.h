// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // EngineMinimal.h
#include "GameFramework/GameModeBase.h"
#include "SH_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASH_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASH_GameModeBase();

	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	// 컨트롤러 정보를 가져와서 스코어 함수 호출
	void AddScore(class ASH_PlayerController* ScoredPlayer);

	// 현재 게임 스테이트에서 토탈 게임 스코어를 가져옴
	int32 GetScore() const;

private:
	// 게임 스테이트 변수
	UPROPERTY()
	class ASH_GameState* SHGameState;

	// 게임 목표 클리어 스코어 변수
	UPROPERTY()
	int32 ScoreToClear;
};
