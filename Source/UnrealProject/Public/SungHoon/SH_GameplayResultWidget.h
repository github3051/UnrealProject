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
	// ���ӽ�����Ʈ ���漱��
	void BindGameState(class ASH_GameState* GameState);

protected:
	// UI�� ��� �غ� �Ϸ�Ǹ� ȣ��Ǵ� �Լ�
	virtual void NativeConstruct() override;

private:
	// ���� ���� ������Ʈ�� weak ptr�� ������ ����
	TWeakObjectPtr<class ASH_GameState> CurrentGameState;
};
