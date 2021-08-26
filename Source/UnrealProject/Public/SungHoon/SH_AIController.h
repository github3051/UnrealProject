// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "AIController.h"
#include "SH_AIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASH_AIController : public AAIController
{
	GENERATED_BODY()

public:
	ASH_AIController();
	// 언리얼 4.22부터 Possess -> OnPossess로 명칭 바뀜
	virtual void OnPossess(APawn* InPawn) override;

	// 블랙보드에서 사용할 키값들을 전역변수로 선언해보자.
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	// 블랙보드
	UPROPERTY()
	class UBlackboardData* BBAsset;
	// 비헤이비어 트리
	UPROPERTY()
	class UBehaviorTree* BTAsset;

};
