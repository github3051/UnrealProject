// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "GameFramework/SaveGame.h"
#include "SH_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_SaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:
	USH_SaveGame();

	UPROPERTY()
	int32 Level;
	UPROPERTY()
	int32 Exp;
	UPROPERTY()
	FString PlayerName;
	UPROPERTY()
	int32 HighScore;

	UPROPERTY()
	int32 CharacterIndex;
};
