// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "BehaviorTree/BTService.h"
#include "SH_BTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_BTService_Detect : public UBTService
{
	GENERATED_BODY()
	

public:
	USH_BTService_Detect();

protected:
	// 자신이 속한 컴포짓 노드가 활성화 될 경우, interval 간격으로 계속해서 자동으로 호출되는 tick 함수.
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
