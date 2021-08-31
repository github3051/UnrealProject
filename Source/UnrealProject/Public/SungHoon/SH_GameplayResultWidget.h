// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "SungHoon/SH_GameplayWidget.h"
#include "SH_GameplayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_GameplayResultWidget : public USH_GameplayWidget
{
	GENERATED_BODY()
	
public:
	// 게임스테이트 전방선언
	void BindGameState(class ASH_GameState* GameState);

protected:
	// UI가 모두 준비 완료되면 호출되는 함수
	virtual void NativeConstruct() override;

private:
	// 현재 게임 스테이트를 weak ptr로 가지고 있음
	TWeakObjectPtr<class ASH_GameState> CurrentGameState;
};
