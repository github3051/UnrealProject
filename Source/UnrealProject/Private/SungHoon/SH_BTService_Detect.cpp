// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_BTService_Detect.h"
#include "SungHoon/SH_AIController.h"
#include "SungHoon/SH_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


USH_BTService_Detect::USH_BTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f; // 1초마다 tick 수행
}

void USH_BTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 현재 비헤이비어트리를 사용하고 있는 폰을 가져옴
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	// 폰을 가져올 수 없으면 종료
	if (ControllingPawn == nullptr) return;

	// 폰이 속한 월드 정보를 가져옴
	UWorld* World = ControllingPawn->GetWorld();
	// 폰의 현재 위치를 가져옴
	FVector Center = ControllingPawn->GetActorLocation();
	// 탐지하는 영역 반경 600mm = 60cm
	float DetectRadius = 600.0f;

	// 월드 정보를 가져올 수 없으면 종료
	if (World == nullptr) return;

	// 반경내 모든 캐릭터 정보를 담을 변수
	TArray<FOverlapResult> OverlapResults;

	// 자기자신은 제외하는 특성을 추가
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);

	// Attack이라는 콜리전 채널을 쓰는 모든 대상이 범위내에 존재하는지 스캔. (공격을 할줄 아는 대상들 모조리 검색)
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults, // 충돌처리 결과를 담을 변수
		Center,			// 어딜 기준으로 스캔할지
		FQuat::Identity,// 회전 상태는 디폴트값으로.
		ECollisionChannel::ECC_GameTraceChannel2, // Attack 콜리전 채널 사용하는 놈 추적
		FCollisionShape::MakeSphere(DetectRadius), // 충돌 탐지할 반경은 구체.
		CollisionQueryParam // 특이사항 저장
	);

	/// TODO : 탐지한 대상이 플레이언지를 판별

	// 탐지된 대상이 있다면
	if (bResult)
	{
		// 일단 TargetKey를 nullptr로 초기화.
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASH_AIController::TargetKey, nullptr);

		// 충돌된 정보를 모조리 돌면서
		for (auto const& OverlapResult : OverlapResults)
		{
			// 해당 액터를 캐스팅
			ASH_Character* SHCharacter = Cast<ASH_Character>(OverlapResult.GetActor());

			// SHCharacter가 유효하고, 그 캐릭터가 플레이어 컨트롤러인지 (플레이어인지)
			if (SHCharacter && SHCharacter->GetController()->IsPlayerController())
			{
				// TargetKey를 플레이어 오브젝트로 지정.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASH_AIController::TargetKey, SHCharacter);
				// 초록색 구체를 그림
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				// target 캐릭터 위치에 파란점을 만듦.
				DrawDebugPoint(World, SHCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				// AI 기준에서 target으로 선을 그음.
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), SHCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASH_AIController::TargetKey, nullptr);
		// 탐지된게 없으면 탐색 구체 반경을 그림
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}
}