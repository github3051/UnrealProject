// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "Blueprint/UserWidget.h"
#include "SH_HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	// CharacterStatComponent 연동
	void BindCharacterStat(class USH_CharacterStatComponent* CharacterStat);
	// PlayerState 정보를 연동
	void BindPlayerState(class ASH_PlayerState* PlayerState);

protected:
	// UI 시스템이 준비 완료되면 한번 호출되는 함수
	virtual void NativeConstruct() override;

	void UpdateCharacterStat();
	void UpdatePlayerState();
private:

private:
	// CharacterStatComponent 변수
	TWeakObjectPtr<class USH_CharacterStatComponent> CurrentCharacterStat;
	// PlayerState 변수
	TWeakObjectPtr<class ASH_PlayerState> CurrentPlayerState;

	// Bar
	UPROPERTY()
	class UProgressBar* HPBar;
	UPROPERTY()
	class UProgressBar* ExpBar;
	// Player
	UPROPERTY()
	class UTextBlock* PlayerName;
	UPROPERTY()
	class UTextBlock* PlayerLevel;
	// Score
	UPROPERTY()
	class UTextBlock* CurrentScore;
	UPROPERTY()
	class UTextBlock* HighScore;
};
