// Fill out your copyright notice in the Description page of Project Settings.



#include "JongGyun/JGGameModeBase.h"
#include "JongGyun/JGPawn.h"
#include "JongGyun/JGPlayerController.h"

AJGGameModeBase::AJGGameModeBase()
{
	DefaultPawnClass = AJGPawn::StaticClass();
	PlayerControllerClass = AJGPlayerController::StaticClass();
}

void AJGGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	JGLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	JGLOG(Warning, TEXT("PostLogin End"));
}