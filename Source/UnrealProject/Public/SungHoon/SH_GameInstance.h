// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "Engine/DataTable.h" // for FTableRowBase clas
#include "Engine/GameInstance.h"
#include "SH_GameInstance.generated.h"


USTRUCT(BlueprintType)
struct FSHCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSHCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};


/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	USH_GameInstance();

	// 게임이 최초 실행되면 자동으로 호출되는 함수.
	virtual void Init() override;

	// 레벨에 맞는 열(Row)의 FSHCharacterData를 반환해줌. 위 구조체.
	FSHCharacterData* GetSHCharacterData(int32 Level);

private:
	// 전방선언 & 데이터 테이블을 읽어올 포인터 변수 선언
	UPROPERTY()
	class UDataTable* SHCharacterTable;

};
