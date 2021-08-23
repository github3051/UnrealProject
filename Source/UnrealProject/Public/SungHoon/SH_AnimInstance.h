// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal.h
#include "Animation/AnimInstance.h"
#include "SH_AnimInstance.generated.h"


// 사용자 정의 멀티캐스트 델리게이트 매크로 자료형 선언
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);



UCLASS()
class UNREALPROJECT_API USH_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USH_AnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// for Montage
	void PlayAttackMontage();

	// 다음 몽타주 섹션으로 넘어가기 위한 함수
	void JumpToAttackMontageSection(int32 NewSection);

public:
	// 델리게이트 변수 선언.
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnNextAttackCheckDelegate OnNextAttackCheck;

	// 죽는 애니메이션 설정
	void SetDeadAnim() { IsDead = true; }

private:
	// 노티파이 함수. _이하의 내용은 노티파이 이름과 정확히 일치해야한다.
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;
};
