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

void ASH_AIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	SH_LOG_S(Error);

	// 블랙보드 사용가능하다면 실행.
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// 블랙보드의 HomePosKey 값을 현재 폰의 위치로 설정하라. (최초위치)
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
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

