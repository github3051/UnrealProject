// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "../../UnrealProject.h" // for EngineMinimal
#include "BehaviorTree/BTTaskNode.h"
#include "SH_BTTask_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_BTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	USH_BTTask_FindPatrolPos();

	// Task 노드가 실행될때마다 자동으로 한번 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
