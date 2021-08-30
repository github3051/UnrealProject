// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_GameModeBase.h"
//#include "SungHoon/SHPawn.h" // added SHpawn.h
#include "SungHoon/SH_PlayerController.h" // SHPlayerController.h
#include "SungHoon/SH_Character.h" // SH_Character.h
#include "SungHoon/SH_PlayerState.h"
#include "SungHoon/SH_GameState.h"

ASH_GameModeBase::ASH_GameModeBase()
{
	// Pawn ����
	DefaultPawnClass = ASH_Character::StaticClass();
	// PlayerController�� ���� Ŭ���� ������ ���Ӹ��� �Ѱ�����.
	PlayerControllerClass = ASH_PlayerController::StaticClass();

	// PlayerState ����
	PlayerStateClass = ASH_PlayerState::StaticClass();
	// GameState ����
	GameStateClass = ASH_GameState::StaticClass();
}

void ASH_GameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Game State �޾ƿ���.
	SHGameState = Cast<ASH_GameState>(GameState);
}

// �÷��̾ �����ϸ� ���� ����� PostLogin �̺�Ʈ �Լ��� ȣ���.
void ASH_GameModeBase::PostLogin(APlayerController * NewPlayer)
{
	// �÷��̾� ��Ʈ�ѷ��� ������ �Ϸ�� ����.

	SH_LOG(Error, TEXT("PostLogin Begin"));
	// �÷��̾ ������ ���� �����ϰ�, �÷��̾� ��Ʈ�ѷ��� �ش� ���� ������
	Super::PostLogin(NewPlayer);
	SH_LOG(Error, TEXT("PostLogin End"));

	// �÷��̾��� PlayerState ������.
	auto SHPlayerState = Cast<ASH_PlayerState>(NewPlayer->PlayerState);
	SH_CHECK(SHPlayerState != nullptr); // ���������� ���.
	// �÷��̾� ������Ʈ �ʱ�ȭ.
	SHPlayerState->InitPlayerData();
}

void ASH_GameModeBase::AddScore(ASH_PlayerController * ScoredPlayer)
{
	// ������� ��� �÷��̾� ��Ʈ�ѷ��� �����ͼ� �ϳ��� ����
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// �÷��̾� ��Ʈ�ѷ��� ĳ����
		const auto SHPlayerController = Cast<ASH_PlayerController>(It->Get());
		// ĳ������ ��������, Score�� ȹ���� �÷��̾� ��Ʈ�ѷ��� �����ϴٸ�
		if ((SHPlayerController != nullptr) && (ScoredPlayer == SHPlayerController))
		{
			// �÷��̾� ��Ʈ�ѷ��� ���� ������ �߰��ϰ� ����
			SHPlayerController->AddGameScore();
			break;
		}
	}

	// ���� ������Ʈ���� ������ �߰�
	SHGameState->AddGameScore();
}

// ���� ���� ������Ʈ���� ��Ż ���� ���ھ ������
int32 ASH_GameModeBase::GetScore() const
{
	return SHGameState->GetTotalGameScore();
}
