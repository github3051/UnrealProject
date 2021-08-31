// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "Blueprint/UserWidget.h"
#include "SH_GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_GameplayWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnResumClicked();
	UFUNCTION()
	void OnReturnToTitleClicked();
	UFUNCTION()
	void OnRetryGameClicked();

protected:
	UPROPERTY()
	class UButton* ResumeButton;
	UPROPERTY()
	class UButton* ReturnToTitleButton;
	UPROPERTY()
	class UButton* RetryGameButton;

};
