// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_BTTask_FindPatrolPos.h"
#include "SungHoon/SH_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

USH_BTTask_FindPatrolPos::USH_BTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}


EBTNodeResult::Type USH_BTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 부모 함수를 실행하고 반환값을 받아준다.
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 컨트롤하고 있는 폰을 가져온다. 해당하는 AI컨트롤러에 접근하고 거기서 폰을 가져옴.
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	// AI 폰 정보를 가져오지 못했다면
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 네비게이션 시스템을 월드로부터 가져온다.
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());

	// 네비게이션 시스템을 가져오지 못한다면
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 테스크를 실행하는 폰의 현재 위치 벡터값을 가져온다.
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ASH_AIController::HomePosKey);

	FNavLocation NextPatrol;

	// 네비게이션 시스템을 통해 랜덤한 값을 가져올 수 있다면 통과. NextPatrol에 그 위치값을 저장
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		// 테스크를 실행하는 폰의 블랙보드 값인 PatroPosKey 값을 다음 이동할 위치값으로 수정해준다.
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASH_AIController::PatrolPosKey, NextPatrol.Location);
		// 성공을 반환
		return EBTNodeResult::Succeeded;
	}
	
	// 랜덤한 값을 받아오지 못했다면 실패
	return EBTNodeResult::Failed;
}