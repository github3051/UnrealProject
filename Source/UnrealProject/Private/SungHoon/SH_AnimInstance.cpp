// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_AnimInstance.h"

USH_AnimInstance::USH_AnimInstance()
{
	CurrentPawnSpeed = 0.0f; // ó������ �ӵ� 0
	IsInAir = false; // ó������ ������ �����ϹǷ�.
	IsDead = false; // ó������ ��������Ƿ�.

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SH_ATTACK_MONTAGE(TEXT(
		"/Game/SungHoon/Animation/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (SH_ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = SH_ATTACK_MONTAGE.Object;
	}


	/*----------
		test
	------------*/
	FString TestMontagePath = TEXT("/Game/SungHoon/Animation/SH_Montage_test.SH_Montage_test");
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SH_TestAttack_Montage(*TestMontagePath);
	if (SH_TestAttack_Montage.Succeeded())
	{
		TestAttackMontage = SH_TestAttack_Montage.Object;
	}
}

void USH_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	// ���� �� �ִ��ν��Ͻ��� ��ӹް� �ִ� ���� ������ ������.
	auto Pawn = TryGetPawnOwner();
	// ���� �������� ���ߴٸ� ����
	if (!::IsValid(Pawn)) return;
	
	// + ���� ����ִٸ�
	if(!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		// Character Ŭ������ GetMovementComponent�� �������� ���ؼ� ������ ĳ������ �Ѱ���.
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}

}

void USH_AnimInstance::PlayAttackMontage()
{
	// �׾����� ����
	SH_CHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}


void USH_AnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	// �׾����� ����
	SH_CHECK(!IsDead);
	// ���������� ������ ��ũ�� �α� ���.
	SH_CHECK(Montage_IsPlaying(AttackMontage));
	// ��Ÿ���� ���� �������� �̵��ϴ� �Լ�
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void USH_AnimInstance::AnimNotify_AttackHitCheck()
{
	SH_LOG_S(Warning);
	// ��ϵ� ��� ��������Ʈ �Լ� ȣ��. ������ ��ϵȰ� ���� ����.
	// ���߿� �浹ó���� �����Ͽ� ������ �����Ҷ� ������ ����. ������ Broadcast�ϴ� ������ ����.
	OnAttackHitCheck.Broadcast();
}

void USH_AnimInstance::AnimNotify_NextAttackCheck()
{
	SH_LOG_S(Warning);
	// ��ϵ� ��� ��������Ʈ �Լ� ȣ��
	OnNextAttackCheck.Broadcast();
}

FName USH_AnimInstance::GetAttackMontageSectionName(int32 Section)
{
	SH_CHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}



////////////////////////////////////////////////
// test
////////////////////////////////////////////////

// ��Ÿ�� ���
void USH_AnimInstance::PlayTestAttackMontage()
{
	// for Debug
	FString currentSectionName = TestAttackMontage->GetSectionName(CurrentSection).ToString();
	SH_LOG(Warning, TEXT("Current Section name : %s"), *currentSectionName);
	
	Montage_Play(TestAttackMontage, 1.0f);
}

// ���� üũ
void USH_AnimInstance::AnimNotify_AccessComboInput()
{
	SH_LOG_S(Warning);
	OnTestAttackHitCheck.Broadcast();
}

// �޺� üũ
void USH_AnimInstance::AnimNotify_TestNextAttackCheck()
{
	SH_LOG_S(Warning);
	OnTestNextAttackCheck.Broadcast();
}

// ���� ��Ÿ�� �������� �̵�
void USH_AnimInstance::JumpToTestAttackMontageSection(int32 NewSection)
{
	// for debug
	CurrentSection = NewSection;
	FName nextSection = *FString::Printf(TEXT("Attack%d"), NewSection);
	SH_LOG(Warning, TEXT("Next Section is : %s"), *(nextSection.ToString()));
	
	// ���� ��Ÿ�� ������ ���
	Montage_JumpToSection(nextSection, TestAttackMontage);
}

void USH_AnimInstance::InitializeSection()
{
	CurrentSection = 0;
}