// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal.h & macro
#include "GameFramework/PlayerController.h"
#include "SH_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASH_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASH_PlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	class USH_HUDWidget* GetHUDWidget() const;
	
	// 플레이어가 NCP를 죽였을때
	void NPCKill(class ASH_Character* KilledNPC) const;
	// 게임 스코어 추가
	void AddGameScore() const;
	// 화면 입력값 전환 함수
	void ChangeInputMode(bool bGameMode = true);

	// 최종 게임 결과 UI 호출함수
	void ShowResultUI();

protected:
	virtual void BeginPlay() override;
	
	// 시스템과 관련된 입력값을 처리하기 위함. 실제 움직임은 캐릭터 폰에서 함.
	virtual void SetupInputComponent() override;

	// HUD 위젯 클래스 전방선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class USH_HUDWidget> HUDWidgetClass;

	// Menu 위젯 클래스 전방선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class USH_GameplayWidget> MenuWidgetClass;

	// Result 위젯 클래스 전방선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class USH_GameplayResultWidget> ResultWidgetClass;

private:
	// 게임을 일시중지하는 함수
	void OnGamePause();

private:
	UPROPERTY()
	class USH_HUDWidget* HUDWidget;

	UPROPERTY()
	class ASH_PlayerState* SHPlayerState;
		
	UPROPERTY()
	class USH_GameplayWidget* MenuWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;

	UPROPERTY()
	class USH_GameplayResultWidget* ResultWidget;
};
