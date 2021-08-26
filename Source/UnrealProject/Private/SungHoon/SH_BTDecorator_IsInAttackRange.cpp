// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_BTDecorator_IsInAttackRange.h"
#include "SungHoon/SH_AIController.h"
#include "SungHoon/SH_Character.h"
#include "BehaviorTree/BlackboardComponent.h"



USH_BTDecorator_IsInAttackRange::USH_BTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool USH_BTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AI 폰을 찾음
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	// 소유하는 폰을 못찾으면 false
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	// Target에 있는 object값을 가져와서 ASH_Character로 캐스팅.
	auto Target = Cast<ASH_Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASH_AIController::TargetKey));
	// 타깃을 못찾으면 false
	if (Target == nullptr)
	{
		return false;
	}

	// 타깃과 현재 AI의 최단거리가 200cm 이하라면 true
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);

	return bResult;
}
