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
	
	// ��Ʈ�ѷ� ������ �����ͼ� ���ھ� �Լ� ȣ��
	void AddScore(class ASH_PlayerController* ScoredPlayer);

	// ���� ���� ������Ʈ���� ��Ż ���� ���ھ ������
	int32 GetScore() const;

private:
	// ���� ������Ʈ ����
	UPROPERTY()
	class ASH_GameState* SHGameState;

	// ���� ��ǥ Ŭ���� ���ھ� ����
	UPROPERTY()
	int32 ScoreToClear;
};
