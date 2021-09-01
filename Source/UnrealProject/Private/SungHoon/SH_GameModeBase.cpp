// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_GameModeBase.h"
//#include "SungHoon/SHPawn.h" // added SHpawn.h
#include "SungHoon/SH_PlayerController.h" // SHPlayerController.h
#include "SungHoon/SH_Character.h" // SH_Character.h
#include "SungHoon/SH_PlayerState.h"
#include "SungHoon/SH_GameState.h"
#include "EngineUtils.h" // for TActorIterator<APawn>

ASH_GameModeBase::ASH_GameModeBase()
{
	// Pawn 설정
	DefaultPawnClass = ASH_Character::StaticClass();
	// PlayerController에 대한 클래스 정보도 게임모드로 넘겨주자.
	PlayerControllerClass = ASH_PlayerController::StaticClass();

	// PlayerState 설정
	PlayerStateClass = ASH_PlayerState::StaticClass();
	// GameState 설정
	GameStateClass = ASH_GameState::StaticClass();

	// 게임 클리어 목표 스코어 2로 지정.
	ScoreToClear = 2;
}

void ASH_GameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Game State 받아오기.
	SHGameState = Cast<ASH_GameState>(GameState);
}

// 플레이어가 입장하면 게임 모드의 PostLogin 이벤트 함수가 호출됨.
void ASH_GameModeBase::PostLogin(APlayerController * NewPlayer)
{
	// 플레이어 컨트롤러의 구성이 완료된 시점.

	SH_LOG(Error, TEXT("PostLogin Begin"));
	// 플레이어가 조종할 폰을 생성하고, 플레이어 컨트롤러가 해당 폰에 빙의함
	Super::PostLogin(NewPlayer);
	SH_LOG(Error, TEXT("PostLogin End"));

	// 플레이어의 PlayerState 가져옴.
	auto SHPlayerState = Cast<ASH_PlayerState>(NewPlayer->PlayerState);
	SH_CHECK(SHPlayerState != nullptr); // 가져왔으면 통과.
	// 플레이어 스테이트 초기화.
	SHPlayerState->InitPlayerData();
}

void ASH_GameModeBase::AddScore(ASH_PlayerController * ScoredPlayer)
{
	// 월드상의 모든 플레이어 컨트롤러를 가져와서 하나씩 접근
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// 플레이어 컨트롤러로 캐스팅
		const auto SHPlayerController = Cast<ASH_PlayerController>(It->Get());
		// 캐스팅이 문제없고, Score를 획득한 플레이어 컨트롤러와 동일하다면
		if ((SHPlayerController != nullptr) && (ScoredPlayer == SHPlayerController))
		{
			// 플레이어 컨트롤러를 통해 점수를 추가하고 종료
			SHPlayerController->AddGameScore();
			break;
		}
	}

	// 게임 스테이트에도 점수를 추가
	SHGameState->AddGameScore();

	// 현재 스코어가 전체 게임 목표 클리어 스코어보다 크면
	if (GetScore() >= ScoreToClear)
	{
		// 클리어 변수를 true로 바꿔줌.
		SHGameState->SetGameCleared();

		// 월드상에 존재하는 모든 폰에 접근
		// cf. http://egloos.zum.com/sweeper/v/3208624
		// for(FConstPawnIterator it = GetWorld()->GetPawnIterator(); It; ++it);
		for (TActorIterator<APawn> It(GetWorld()); It; ++It)
		{
			SH_LOG(Warning, TEXT("Found Actor name : %s"), *(*It)->GetName());
			// 전부 끔. 소리, 물리 등등 동작 중단.
			(*It)->TurnOff();
		}

		// 월드상의 모든 캐릭터 컨트롤러에 접근 -> 멀티플레이어 아니면, 싱글은 1명뿐임.
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			// 캐스팅함.
			const auto SHPlayerController = Cast<ASH_PlayerController>(It->Get());

			if (SHPlayerController != nullptr)
			{
				// 모든 플레이어 컨트롤러에게 결과 화면을 보여줌.
				SHPlayerController->ShowResultUI();
			}
		}
	}

}

// 현재 게임 스테이트에서 토탈 게임 스코어를 가져옴
int32 ASH_GameModeBase::GetScore() const
{
	return SHGameState->GetTotalGameScore();
}
