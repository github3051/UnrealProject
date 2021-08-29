// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal.h
#include "GameFramework/Actor.h"
#include "SH_Weapon.generated.h"

UCLASS()
class UNREALPROJECT_API ASH_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASH_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:


public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

};
