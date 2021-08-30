// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_PlayerState.h"
#include "SungHoon/SH_GameInstance.h" // for FSH_CharacterData
#include "SungHoon/SH_SaveGame.h"


ASH_PlayerState::ASH_PlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	// 점수
	GameHighScore = 0;
	Exp = 0;
	// 슬롯 이름
	SaveSlotName = TEXT("Player1");
}

int32 ASH_PlayerState::GetGameScore() const
{
	return GameHighScore;
}

int32 ASH_PlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

int32 ASH_PlayerState::GetGameHighScore() const
{
	return int32();
}

void ASH_PlayerState::InitPlayerData()
{
	// SaveGame 파일을 폴더에서 슬롯 네임으로 가져와 캐스팅함
	auto SHSaveGame = Cast<USH_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	// SaveGame가 없다면
	if (SHSaveGame == nullptr)
	{
		// 새로운 SaveGame을 생성
		SHSaveGame = GetMutableDefault<USH_SaveGame>();
	}

	// 플레이어의 별명 설정
	SetPlayerName(SHSaveGame->PlayerName);
	SetCharacterLevel(SHSaveGame->Level);
	GameScore = 0;
	GameHighScore = SHSaveGame->HighScore;
	Exp = SHSaveGame->Exp;

	SavePlayerData();
}

// 실제 데이터 게임 세이브하는 함수
void ASH_PlayerState::SavePlayerData()
{
	// UObject 생성
	USH_SaveGame* NewPlayerData = NewObject<USH_SaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;

	// 게임 슬롯에 해당하는 데이터가 이미 있으면 에러. 덮어쓰기.
	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		SH_LOG(Error, TEXT("SaveGame Error!"));
	}
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
	if (CurrentStatData->NextExp == -1)
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
	
	// 경험치를 먹어도 현재 상태를 저장함
	SavePlayerData();
	return DidLevelUp;
}


// GameScore 추가
void ASH_PlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}

	OnPlayerStateChanged.Broadcast();

	// 게임 스코어가 바뀌어도 저장.
	SavePlayerData();
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
