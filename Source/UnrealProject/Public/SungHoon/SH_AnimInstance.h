// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal.h
#include "Animation/AnimInstance.h"
#include "SH_AnimInstance.generated.h"


// ����� ���� ��Ƽĳ��Ʈ ��������Ʈ ��ũ�� �ڷ��� ����
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

	// ���� ��Ÿ�� �������� �Ѿ�� ���� �Լ�
	void JumpToAttackMontageSection(int32 NewSection);

public:
	// ��������Ʈ ���� ����.
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnNextAttackCheckDelegate OnNextAttackCheck;

	// test
	FOnTestAttackHitCheckDelegate OnTestAttackHitCheck;
	FOnTestNextAttackCheckDelegate OnTestNextAttackCheck;

	// �״� �ִϸ��̼� ����
	void SetDeadAnim() { IsDead = true; }

private:
	/*-----------------
		��Ƽ���� �Լ�.
	_������ ������ ��Ƽ���� �̸��� ��Ȯ�� ��ġ�ؾ��Ѵ�.
	------------------*/
	// ���ݿ� ���� �浹 üũ�� Ȯ���� ���� Ÿ�̹�
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	// ���� ���ݿ� ���� ó���� �ϰԵ� �Ǻ� Ÿ�̹�
	UFUNCTION()
	void AnimNotify_NextAttackCheck();
	// ���� �������� ��Ÿ�� ������ �̸��� ��ȯ
	FName GetAttackMontageSectionName(int32 Section);

	//// test
	UFUNCTION()
	void AnimNotify_TestAttackHitCheck();
	UFUNCTION()
	void AnimNotify_TestNextAttackCheck();
	FName GetTestAttackMontageSectionName(int32 Section);
	

private:
	// �޺� ������ ���� ��Ÿ�� �ּ� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	//// test
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TestAttackMontage;

	int32 CurrentSection = 0;

	/*-----------------
		���� ����
	------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;
};
