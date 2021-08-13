// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // EngineMinimal.h
#include "GameFramework/GameModeBase.h"
#include "SHGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASHGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASHGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};