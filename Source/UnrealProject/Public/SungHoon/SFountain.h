// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../UnrealProject.h" // Added Module & Macro for log.
#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h" // RotatingMovement

// 항상 generated.h는 맨 아래에 있어야한다.
#include "SFountain.generated.h" // 언리얼 오브젝트 정의3 : generated.h 헤더 추가


UCLASS() // 언리얼 오브젝트 정의1 : 클래스 선언 매크로
class UNREALPROJECT_API ASFountain : public AActor // 언리얼 오브젝트 정의2 : 클래스 이름 매크로 // 언리얼 오브젝트 정의4 : 외부 모듈에서의 공개 여부
{
	GENERATED_BODY() // 언리얼 오브젝트 정의1 : 클래스 선언 매크로
	
public:	
	// Sets default values for this actor's properties
	ASFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Water;

	// 조명
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;

	// 이펙트
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Splash;

	UPROPERTY(EditAnywhere, Category = ID)
	int32 ID;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;

	// test Log
	void LOG_TEST();

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float RotateSpeed;
};
