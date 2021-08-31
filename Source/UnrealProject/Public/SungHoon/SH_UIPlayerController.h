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

	// �������Ʈ �����Ϳ����� �۾�����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> UIWidgetClass;

	// ���� ���� �ν��Ͻ� ������
	UPROPERTY()
	class UUserWidget* UIWidgetInstance;
};
