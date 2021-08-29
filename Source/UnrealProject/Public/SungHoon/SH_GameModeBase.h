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


private:
	// 게임 스테이트 변수
	UPROPERTY()
	class ASH_GameState* SHGameState;

};
