// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_BTTask_Attack.h"
#include "SungHoon/SH_AIController.h"
#include "SungHoon/SH_Character.h"


USH_BTTask_Attack::USH_BTTask_Attack()
{
	// TickTask 함수 호출되도록 함. Tick 활성화
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type USH_BTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI 폰을 가져옴
	auto SHCharacter = Cast<ASH_Character>(OwnerComp.GetAIOwner()->GetPawn());
	// 가져오지 못했다면 실패
	if (SHCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// AI가 공격을 수행
	SHCharacter->Attack();
	IsAttacking = true;
	// 캐릭터의 OnAttackEnd 델리게이트에 바인딩함. 애니메이션이 종료되면 호출
	SHCharacter->OnAttackEnd.AddLambda([this]() ->void {
		IsAttacking = false;
	});

	// 진행중임을 알림
	return EBTNodeResult::InProgress;
}

// Tick 함수
void USH_BTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	// 공격중이지 않으면 실행
	if (!IsAttacking)
	{
		// Task가 끝났다는것을 알려주는 함수.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
