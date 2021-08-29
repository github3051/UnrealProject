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

protected:
	virtual void BeginPlay() override;
	//virtual void SetupInputComponent() override;

	// HUD 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class USH_HUDWidget> HUDWidgetClass;

private:


private:
	UPROPERTY()
	class USH_HUDWidget* HUDWidget;

	UPROPERTY()
	class ASH_PlayerState* SHPlayerState;
		
};
