// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


// 전역변수 초기화
const FName ASH_AIController::HomePosKey(TEXT("HomePos"));
const FName ASH_AIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ASH_AIController::TargetKey(TEXT("Target"));


ASH_AIController::ASH_AIController()
{
	// 블랙보드 연결
	static ConstructorHelpers::FObjectFinder<UBlackboardData> SH_BBObject(TEXT(
		"/Game/SungHoon/AI/BB_SHCharacter.BB_SHCharacter"
	));
	if (SH_BBObject.Succeeded())
	{
		BBAsset = SH_BBObject.Object;
	}

	// 비헤이비어 트리 연결
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> SH_BTObject(TEXT(
		"/Game/SungHoon/AI/BT_SHCharacter.BT_SHCharacter"
	));
	if (SH_BTObject.Succeeded())
	{
		BTAsset = SH_BTObject.Object;
	}
}

// 컨트롤러가 AI 폰을 빙의했을때 먼저 호출.
void ASH_AIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	SH_LOG_S(Error);
}

// 비헤이비어 트리 실행
void ASH_AIController::RunAI()
{
	// 블랙보드 사용가능하다면 실행.
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// 블랙보드의 HomePosKey 값을 현재 폰의 위치로 설정하라. (최초위치)
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		// 만약 비헤이비어 트리를 작동할 수 없다면
		if (!RunBehaviorTree(BTAsset))
		{
			// 로그 찍기
			SH_LOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
		else
		{
			SH_LOG(Error, TEXT("RunBehaviorTree!"))
		}
	}
}

// 비헤이비어 트리 중단
void ASH_AIController::StopAI()
{
	// BrainComponent :AIController에 있는 멤버변수. 비헤이비어트리 컴포넌트를 가져옴
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	
	if (BehaviorTreeComponent != nullptr)
	{
		// 비헤이비어트리 중단.
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
