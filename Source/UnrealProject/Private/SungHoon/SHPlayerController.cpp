// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SHPlayerController.h"

void ASHPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SH_LOG_S(Warning);
}

void ASHPlayerController::OnPossess(APawn * aPawn)
{
	SH_LOG_S(Warning);
	Super::OnPossess(aPawn);
}
