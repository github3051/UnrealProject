// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_PlayerState.h"
#include "SungHoon/SH_GameInstance.h" // for FSH_CharacterData
#include "SungHoon/SH_SaveGame.h"


ASH_PlayerState::ASH_PlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	// ����
	GameHighScore = 0;
	Exp = 0;
	// ���� �̸�
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
	// SaveGame ������ �������� ���� �������� ������ ĳ������
	auto SHSaveGame = Cast<USH_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	// SaveGame�� ���ٸ�
	if (SHSaveGame == nullptr)
	{
		// ���ο� SaveGame�� ����
		SHSaveGame = GetMutableDefault<USH_SaveGame>();
	}

	// �÷��̾��� ���� ����
	SetPlayerName(SHSaveGame->PlayerName);
	SetCharacterLevel(SHSaveGame->Level);
	GameScore = 0;
	GameHighScore = SHSaveGame->HighScore;
	Exp = SHSaveGame->Exp;

	SavePlayerData();
}

// ���� ������ ���� ���̺��ϴ� �Լ�
void ASH_PlayerState::SavePlayerData()
{
	// UObject ����
	USH_SaveGame* NewPlayerData = NewObject<USH_SaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;

	// ���� ���Կ� �ش��ϴ� �����Ͱ� �̹� ������ ����. �����.
	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		SH_LOG(Error, TEXT("SaveGame Error!"));
	}
}

// ���� ����ġ ���� ��ȯ
float ASH_PlayerState::GetExpRatio() const
{
	// ���� ����ġ ������ �ſ� ���� ���̸�
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	// ������ �Ϸ�ƴ��� �Ҽ���
	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	// �α� ���
	SH_LOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);

	return Result;
}

// ����ġ �߰�
bool ASH_PlayerState::AddExp(int32 IncomeExp)
{
	// ���� ����ġ ���� -1�̸� ���� (�����̶� �ǹ�. ����ġ ������)
	if (CurrentStatData->NextExp == -1)
		return false;

	// ������ ����
	bool DidLevelUp = false;
	// �߰� ����ġ ���
	Exp = Exp + IncomeExp;

	// �߰��� ���� ����ġ�� �ʿ����ġ���� �Ѿ��ٸ� ������!
	if (Exp >= CurrentStatData->NextExp)
	{
		// ����ġ �缳��
		Exp -= CurrentStatData->NextExp;
		// ������
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	// �÷��̾��� ���°� �ٲ�. �缳���� ���� �Լ� ȣ��
	OnPlayerStateChanged.Broadcast();
	
	// ����ġ�� �Ծ ���� ���¸� ������
	SavePlayerData();
	return DidLevelUp;
}


// GameScore �߰�
void ASH_PlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}

	OnPlayerStateChanged.Broadcast();

	// ���� ���ھ �ٲ� ����.
	SavePlayerData();
}

// ĳ���� ���� �缳��
void ASH_PlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	// ���� �ν��Ͻ��� ������
	auto SHGameInstance = Cast<USH_GameInstance>(GetGameInstance());
	SH_CHECK(SHGameInstance != nullptr);

	// �ش� ������ �´� �׼������͸� ����ü ������ �̾Ƽ� ������
	CurrentStatData = SHGameInstance->GetSHCharacterData(NewCharacterLevel);
	SH_CHECK(CurrentStatData != nullptr);

	// ���� ĳ���� ���� ����
	CharacterLevel = NewCharacterLevel;
}
