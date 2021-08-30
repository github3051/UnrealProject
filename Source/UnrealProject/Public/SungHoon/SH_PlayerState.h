// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h"
#include "GameFramework/PlayerState.h"
#include "SH_PlayerState.generated.h"

// ��������Ʈ ����
DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);


/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASH_PlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:
	ASH_PlayerState();

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;

	// for Game Save
	int32 GetGameHighScore() const;
	FString SaveSlotName;

	// exp ����
	float GetExpRatio() const; // ���� ����ġ ���� ��ȯ
	bool AddExp(int32 IncomeExp);

	// for GameScore
	void AddGameScore();

	// �÷��̾� ������ �ʱ�ȭ �Լ�
	void InitPlayerData();

	// ����
	void SavePlayerData();

	// ��������Ʈ ���� ����
	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 GameHighScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	// ����ġ ����
	UPROPERTY(Transient)
	int32 Exp;

private:
	// ���� ����
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FSHCharacterData* CurrentStatData;
};
