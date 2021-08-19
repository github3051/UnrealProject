// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SHGameModeBase.h"
#include "SungHoon/SHPawn.h" // added SHpawn.h
#include "SungHoon/SHPlayerController.h" // SHPlayerController.h
#include "SungHoon/SH_Character.h" // SH_Character.h

ASHGameModeBase::ASHGameModeBase()
{
	// DefaultPawnClass 등록, pawn 클래스 정보를 게임모드 폰 클래스 변수에 저장.
	// DefaultPawnClass = ASHPawn::StaticClass();
	DefaultPawnClass = ASH_Character::StaticClass();

	// BP로 만든 폰을 넘기려면 아래처럼 한다.
	/*
	static ConstructorHelpers::FClassFinder<APawn>
		BP_PAWN_SH(TEXT("/Game/ThirdPersonBP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter_C"));

	if (BP_PAWN_SH.Succeeded())
	{
		// DefaultPawnClass를 BP 폰 클래스 정보로 등록.
		DefaultPawnClass = BP_PAWN_SH.Class;
	}
	*/

	// PlayerController에 대한 클래스 정보도 게임모드로 넘겨주자.
	PlayerControllerClass = ASHPlayerController::StaticClass();
}

// 플레이어가 입장하면 게임 모드의 PostLogin 이벤트 함수가 호출됨.
void ASHGameModeBase::PostLogin(APlayerController * NewPlayer)
{
	SH_LOG(Error, TEXT("PostLogin Begin"));
	
	// 플레이어가 조종할 폰을 생성하고, 플레이어 컨트롤러가 해당 폰에 빙의함
	Super::PostLogin(NewPlayer);
	
	SH_LOG(Error, TEXT("PostLogin End"));
}
