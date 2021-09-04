// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_AnimInstance.h"

USH_AnimInstance::USH_AnimInstance()
{
	CurrentPawnSpeed = 0.0f; // 처음에는 속도 0
	IsInAir = false; // 처음에는 땅에서 시작하므로.
	IsDead = false; // 처음에는 살아있으므로.

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


	// 현재 이 애님인스턴스를 상속받고 있는 폰의 정보를 가져옴.
	auto Pawn = TryGetPawnOwner();
	// 만약 가져오지 못했다면 종료
	if (!::IsValid(Pawn)) return;
	
	// + 폰이 살아있다면
	if(!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		// Character 클래스의 GetMovementComponent를 가져오기 위해서 억지로 캐스팅을 한거임.
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}

}

void USH_AnimInstance::PlayAttackMontage()
{
	// 죽었으면 종료
	SH_CHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}


void USH_AnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	// 죽었으면 종료
	SH_CHECK(!IsDead);
	// 실행중이지 않으면 매크로 로그 출력.
	SH_CHECK(Montage_IsPlaying(AttackMontage));
	// 몽타주의 다음 섹션으로 이동하는 함수
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void USH_AnimInstance::AnimNotify_AttackHitCheck()
{
	SH_LOG_S(Warning);
	// 등록된 모든 델리게이트 함수 호출. 아직은 등록된게 전혀 없다.
	// 나중에 충돌처리와 관련하여 로직을 구현할때 수정될 내용. 지금은 Broadcast하는 내용이 없다.
	OnAttackHitCheck.Broadcast();
}

void USH_AnimInstance::AnimNotify_NextAttackCheck()
{
	SH_LOG_S(Warning);
	// 등록된 모든 델리게이트 함수 호출
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

// 몽타주 재생
void USH_AnimInstance::PlayTestAttackMontage()
{
	// for Debug
	FString currentSectionName = TestAttackMontage->GetSectionName(CurrentSection).ToString();
	SH_LOG(Warning, TEXT("Current Section name : %s"), *currentSectionName);
	
	Montage_Play(TestAttackMontage, 1.0f);
}

// 공격 체크
void USH_AnimInstance::AnimNotify_AccessComboInput()
{
	SH_LOG_S(Warning);
	OnTestAttackHitCheck.Broadcast();
}

// 콤보 체크
void USH_AnimInstance::AnimNotify_TestNextAttackCheck()
{
	SH_LOG_S(Warning);
	OnTestNextAttackCheck.Broadcast();
}

// 다음 몽타주 섹션으로 이동
void USH_AnimInstance::JumpToTestAttackMontageSection(int32 NewSection)
{
	// for debug
	CurrentSection = NewSection;
	FName nextSection = *FString::Printf(TEXT("Attack%d"), NewSection);
	SH_LOG(Warning, TEXT("Next Section is : %s"), *(nextSection.ToString()));
	
	// 다음 몽타주 섹션을 재생
	Montage_JumpToSection(nextSection, TestAttackMontage);
}

void USH_AnimInstance::InitializeSection()
{
	CurrentSection = 0;
}