// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "GameFramework/Actor.h"
#include "SH_Section.generated.h"

UCLASS()
class UNREALPROJECT_API ASH_Section : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASH_Section();

	// 에디터 작업에서 선택한 액터의 속성이나 트랜스폼 정보가 변결될때 자동 호출되는 함수
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 섹션 state를 위한 열거형 변수
	enum class ESectionState : uint8
	{
		READY = 0,
		BATTLE,
		COMPLETE
	};

	// 섹션 설정
	void SetState(ESectionState NewState);
	// 섹션 변수.
	ESectionState CurrentState = ESectionState::READY;
	// 문을 열고 닫는 함수. 입력값 없으면 기본 true
	void OperateGates(bool bOpen = true);

	// 중앙 큰 박스
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 각 문에 있는 박스
	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// NPC 생성함수
	void OnNPCSpawn();

	// NPC가 파괴되었을때 호출할 함수
	UFUNCTION()
	void OnKeyNPCDestroyed(AActor* DestoryedActor);

private:
	// for SM_SQUARE
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Mesh;

	// for Box Trigger
	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrivateAccess = true))
	UBoxComponent* Trigger;

	// for SM_GATE
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
	TArray<UStaticMeshComponent*> GateMeshes;

	// for Box Trigger
	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrivateAccess = true))
	TArray<UBoxComponent*> GateTriggers;

	// 전투 없이 바로 완료 스테이트에서 시작할 수 있는 변수
	UPROPERTY(EditAnywhere, Category = State, Meta = (AllowPrivateAccess = true))
	bool bNoBattle;


	// AI Spawn Time
	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	float EnemySpawnTime;

	// Item Spawn Time
	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	float ItemBoxSpawnTime;

	// 타이머 핸들 변수
	FTimerHandle SpawnNPCTimerHandle = {};
	FTimerHandle SpawnItemBoxTimerHandle = {};
};
