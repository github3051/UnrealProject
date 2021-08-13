// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SHPawn.h"

// Sets default values
ASHPawn::ASHPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASHPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASHPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASHPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SH_LOG_S(Warning);
}

void ASHPawn::PossessedBy(AController* NewController)
{
	SH_LOG_S(Warning);
	Super::PossessedBy(NewController);
}

// Called to bind functionality to input
void ASHPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
