// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "BehaviorTree/BTTaskNode.h"
#include "SH_BTTask_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_BTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	USH_BTTask_TurnToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
