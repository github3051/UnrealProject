// Fill out your copyright notice in the Description page of Project Settings.


#include "JongGyun/JGPlayerController.h"

void AJGPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	JGLOG_S(Warning);
}

void AJGPlayerController::OnPossess(APawn * aPawn)
{

	JGLOG_S(Warning);
	Super::OnPossess(aPawn);
}

void AJGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}