// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal.h & macro
#include "GameFramework/PlayerController.h"
#include "SHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASHPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
};
