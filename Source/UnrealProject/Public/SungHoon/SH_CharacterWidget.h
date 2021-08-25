// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "Blueprint/UserWidget.h"
#include "SH_CharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_CharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void BindCharacterStat(class USH_CharacterStatComponent* NewCharacterStat);

protected:
	// UI 시스템이 준비되면 호출되는 함수
	virtual void NativeConstruct() override;
	// HPBar 위젯 업데이트 함수
	void UpdateHPWidget();

private:
	// 약포인터 선언. 전방선언임
	TWeakObjectPtr<class USH_CharacterStatComponent> CurrentCharacterStat;

	// ProgressBar에 대한 전방선언 및 포인터 변수 선언
	UPROPERTY()
	class UProgressBar* HPProgressBar;

};
