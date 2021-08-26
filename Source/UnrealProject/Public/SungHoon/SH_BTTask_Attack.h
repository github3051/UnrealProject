// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "BehaviorTree/BTTaskNode.h"
#include "SH_BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_BTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	USH_BTTask_Attack();
	// 노드가 실행될때 실행되는 함수.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// 노드의 Tick 함수
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	// AI가 공격중인지를 판별하는 변수
	bool IsAttacking;
};
