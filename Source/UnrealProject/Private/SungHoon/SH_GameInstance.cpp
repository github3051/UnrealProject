// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_GameInstance.h"

USH_GameInstance::USH_GameInstance()
{
	// 이렇게 경로를 문자열 변수로(FString) 들고 있을수도 있음. 내부는 포인터
	FString CharacterDataPath = TEXT(
		"/Game/SungHoon/GameData/ABCharacterData.ABCharacterData");

	// 위의 문자열을 이용해서 애셋을 찾아옴. 이때 문자열값의 *로 적어줘야함. 실제 내용(값)을 표현하기위해.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SHCHARACTER(*CharacterDataPath);

	// 성공적으로 받아와졌으면 통과
	SH_CHECK(DT_SHCHARACTER.Succeeded());
	// UDataTable 변수에 등록.
	SHCharacterTable = DT_SHCHARACTER.Object;
	// 잘들어왔다면, 테이블의 RowMap을 가져와 숫자를 읽고, 그 값이 0보다 크면 통과.
	SH_CHECK(SHCharacterTable->GetRowMap().Num() > 0);
}

void USH_GameInstance::Init()
{
	Super::Init();
	// SHCharacterTable로 받아온 실제 데이터 정보에서, 20번째 행에 접근하여 열의 데이터를 가져온다.
	SH_LOG(Error, TEXT("DropExp of Level 20 SHCharactor : %d"), GetSHCharacterData(20)->DropExp);
}

FSHCharacterData * USH_GameInstance::GetSHCharacterData(int32 Level)
{
	// int형을 string으로 바꾸는 FString::FromInt(int), 해당하는 행의 row 데이터를 가져오는 FindRow함수.
	return SHCharacterTable->FindRow<FSHCharacterData>(*FString::FromInt(Level), TEXT(""));
}
