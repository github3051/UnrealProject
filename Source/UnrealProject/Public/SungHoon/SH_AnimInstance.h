// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal.h
#include "Animation/AnimInstance.h"
#include "SH_AnimInstance.generated.h"


// 사용자 정의 멀티캐스트 델리게이트 매크로 자료형 선언
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);

// test
DECLARE_MULTICAST_DELEGATE(FOnTestAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnTestNextAttackCheckDelegate);



UCLASS()
class UNREALPROJECT_API USH_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USH_AnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// for Montage
	void PlayAttackMontage();

	// test
	void PlayTestAttackMontage();
	void JumpToTestAttackMontageSection(int32 NewSection);
	void SetCurrentSection();

	// 다음 몽타주 섹션으로 넘어가기 위한 함수
	void JumpToAttackMontageSection(int32 NewSection);

public:
	// 델리게이트 변수 선언.
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnNextAttackCheckDelegate OnNextAttackCheck;

	// test
	FOnTestAttackHitCheckDelegate OnTestAttackHitCheck;
	FOnTestNextAttackCheckDelegate OnTestNextAttackCheck;

	// 죽는 애니메이션 설정
	void SetDeadAnim() { IsDead = true; }

private:
	/*-----------------
		노티파이 함수.
	_이하의 내용은 노티파이 이름과 정확히 일치해야한다.
	------------------*/
	// 공격에 대한 충돌 체크를 확인할 순간 타이밍
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	// 다음 공격에 대한 처리를 하게될 판별 타이밍
	UFUNCTION()
	void AnimNotify_NextAttackCheck();
	// 현재 진행중인 몽타주 섹션의 이름을 반환
	FName GetAttackMontageSectionName(int32 Section);

	//// test
	UFUNCTION()
	void AnimNotify_TestAttackHitCheck();
	UFUNCTION()
	void AnimNotify_TestNextAttackCheck();
	FName GetTestAttackMontageSectionName(int32 Section);
	

private:
	// 콤보 공격을 위한 몽타주 애셋 정보
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	//// test
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TestAttackMontage;

	int32 CurrentSection = 0;

	/*-----------------
		상태 변수
	------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;
};
