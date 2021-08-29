// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_PlayerState.h"
#include "SungHoon/SH_GameInstance.h" // for FSH_CharacterData


ASH_PlayerState::ASH_PlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
}

int32 ASH_PlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ASH_PlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

void ASH_PlayerState::InitPlayerData()
{
	// 플레이어의 별명 설정
	SetPlayerName(TEXT("SungHoon"));
	SetCharacterLevel(5);
	GameScore = 0;
	Exp = 0;
}

// 현재 경험치 비율 반환
float ASH_PlayerState::GetExpRatio() const
{
	// 다음 경험치 정보가 매우 작은 값이면
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	// 몇프로 완료됐는지 소숫점
	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	// 로그 찍기
	SH_LOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
	
	return Result;
}

// 경험치 추가
bool ASH_PlayerState::AddExp(int32 IncomeExp)
{
	// 다음 경험치 값이 -1이면 종료 (만렙이란 의미. 경험치 못먹음)
	if(CurrentStatData->NextExp==-1)
		return false;

	// 레벨업 변수
	bool DidLevelUp = false;
	// 추가 경험치 계산
	Exp = Exp + IncomeExp;

	// 추가된 현재 경험치가 필요경험치량을 넘었다면 레벨업!
	if (Exp >= CurrentStatData->NextExp)
	{
		// 경험치 재설정
		Exp -= CurrentStatData->NextExp;
		// 레벨업
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	// 플레이어의 상태가 바뀜. 재설정을 위한 함수 호출
	OnPlayerStateChanged.Broadcast();
	return DidLevelUp;
}


// GameScore 추가
void ASH_PlayerState::AddScore()
{
	GameScore++;
	OnPlayerStateChanged.Broadcast();
}

// 캐릭터 레벨 재설정
void ASH_PlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	// 게임 인스턴스를 가져옴
	auto SHGameInstance = Cast<USH_GameInstance>(GetGameInstance());
	SH_CHECK(SHGameInstance != nullptr);

	// 해당 레벨에 맞는 액셀데이터를 구조체 변수로 뽑아서 가져옴
	CurrentStatData = SHGameInstance->GetSHCharacterData(NewCharacterLevel);
	SH_CHECK(CurrentStatData != nullptr);

	// 현재 캐릭터 레벨 수정
	CharacterLevel = NewCharacterLevel;
}
