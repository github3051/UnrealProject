// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "Components/ActorComponent.h"
#include "SH_CharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API USH_CharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USH_CharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// PostInitializeComponents 함수가 실행되기전에 호출된다.
	virtual void InitializeComponent() override;

public:
	// 레벨 설정 함수, 일종의 레벨업 함수.
	void SetNewLevel(int32 NewLevel);
	// 데미지를 받은것을 처리함.
	void SetDamage(float NewDamage);
	// HP 설정
	void SetHP(float NewHP);
	// 현재 HP % 비율
	float GetHPRatio() const;
	// 해당 레벨의 player의 공격력을 가져옴
	float GetAttack() const;
	// 몬스터 경험치 반환함수
	int32 GetDropExp() const;

	// 델리게이트 변수
	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

private:
	// 구조체 전방선언
	struct FSHCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;
	// Transient는 직렬화를 제외시키는 키워드
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
		
};
