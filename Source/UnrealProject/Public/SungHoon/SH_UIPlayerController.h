// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "GameFramework/PlayerController.h"
#include "SH_UIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASH_UIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	// 블루프린트 에디터에서만 작업가능
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> UIWidgetClass;

	// 실제 위젯 인스턴스 포인터
	UPROPERTY()
	class UUserWidget* UIWidgetInstance;
};
