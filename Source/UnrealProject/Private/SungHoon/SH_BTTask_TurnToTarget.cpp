// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_BTTask_TurnToTarget.h"
#include "SungHoon/SH_AIController.h"
#include "SungHoon/SH_Character.h"
#include "BehaviorTree/BlackboardComponent.h"



USH_BTTask_TurnToTarget::USH_BTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}


EBTNodeResult::Type USH_BTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 실행하고 있는 AI의 폰을 가져옴
	auto SHCharacter = Cast<ASH_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (SHCharacter == nullptr)
	{
		EBTNodeResult::Failed;
	}

	// 블랙보드의 Target에 들어가있는 Object 데이터를 가져와서 캐스팅
	auto Target = Cast<ASH_Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASH_AIController::TargetKey));
	if (Target == nullptr)
	{
		EBTNodeResult::Failed;
	}

	// 방향벡터 구하기. 뺀놈에서 Target으로 바라봄. AI가 Target을 바라보는 방향벡터임.
	FVector LookVector = Target->GetActorLocation() - SHCharacter->GetActorLocation();
	
	LookVector.Z = 0.0f; // Z값 0으로 초기화
	// 바라보는 방향벡터쪽으로 앞방향 회전행렬을 구함
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	// 회전값 설정
	SHCharacter->SetActorRotation(FMath::RInterpTo(SHCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));
	
	return EBTNodeResult::Succeeded;
}