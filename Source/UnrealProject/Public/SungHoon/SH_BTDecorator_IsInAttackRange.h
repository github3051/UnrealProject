// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "BehaviorTree/BTDecorator.h"
#include "SH_BTDecorator_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_BTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	USH_BTDecorator_IsInAttackRange();

protected:
	// 원하는 조건이 달성됐는지 파악하는 함수.
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
