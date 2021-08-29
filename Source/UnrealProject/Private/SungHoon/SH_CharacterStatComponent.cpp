// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_CharacterStatComponent.h"
#include "SungHoon/SH_GameInstance.h"

// Sets default values for this component's properties
USH_CharacterStatComponent::USH_CharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; // tick을 꺼주자.
	bWantsInitializeComponent = true; // InitializeComponent 함수를 켜달라.


	Level = 1;
}


// Called when the game starts
void USH_CharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void USH_CharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	// 초기화
	SetNewLevel(Level);
}

// 레벨업 함수
void USH_CharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	// GameInstance클래스를 가져올때 프로젝트 셋팅에서 설정해놓은 GameInstance를 가져오고 있다.
	// 만약 프로젝트 셋팅으로 한게 아니라면, 전역변수 등등으로 가져와야할듯.
	// 프로젝트 셋팅에 해놓는건 일종의 싱글톤 효과라고 보면됨. 워낙 많이 쓰니까 언리얼에서 하나 만들어준거임.
	auto SHGameInstance = Cast<USH_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// SHGameInstance를 제대로 가져왔다면
	SH_CHECK(SHGameInstance != nullptr);

	// Level에 해당하는 열의 row 값들을 가져올 수 있다.
	// 이 row를 받는 자료형은 구조체변수임. 거기에 똑같은 row의 자료형이 있음.
	CurrentStatData = SHGameInstance->GetSHCharacterData(NewLevel);

	if (CurrentStatData != nullptr)
	{
		// 현재 레벨을 새로운 레벨로 직접 설정
		Level = NewLevel;
		// 현재 HP는 새로운 레벨(NewLevel)에 해당하는 MaxHP값으로 수정. 풀피됨
		SetHP(CurrentStatData->MaxHP); // 레벨업 상황이라고 보면됨.
	}
	else
	{
		// CurrentStatData가 값을 가리키고 있지 않으면
		SH_LOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

void USH_CharacterStatComponent::SetDamage(float NewDamage)
{
	// CurrentStatData를 잘 가지고 있으면 통과.
	// 최초 1레벨때 초기화되면서 가지게 됨. InitializeComponent에서
	SH_CHECK(CurrentStatData != nullptr);
	// 데미지가 들어온것을 최소 0 ~ 최대 HP 값 사이로 한정지음. 0 밑으로 못가게 막는 역할.
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void USH_CharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	// OnHPChanged와 바인딩된 델리게이트 함수들 모두 호출한다. 이벤트 발동!
	OnHPChanged.Broadcast();
	// 현재HP가 0이라면 (모두 소진했다면)
	// KINDA_SMALL_NUMBER : 0과 비교할때 무시 가능한 오차를 측정할때 사용. (=매우 작은 양수)
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		// 매우작은 양수 이하라면 0으로 셋팅
		CurrentHP = 0.0f;
		// OnHPIsZero와 바인딩된 델리게이트 함수들 모두 호출한다. 이벤트 발동!
		OnHPIsZero.Broadcast();
	}
}

float USH_CharacterStatComponent::GetHPRatio() const
{
	// CurrentStatData가 정상적으로 있다면 통과
	SH_CHECK(CurrentStatData != nullptr, 0.0f);
	// 매우 작은 양수 이하라면 0으로 출력, 그게 아니라면 현재 HP / 최대 HP => 소수점으로 나옴. x100하면 백분위%임.
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

// 해당 레벨의 player의 공격력을 가져옴
float USH_CharacterStatComponent::GetAttack() const
{
	// CurrentStatData를 잘 가지고 있으면 통과.
	// 최초 1레벨때 초기화되면서 가지게 됨. InitializeComponent에서
	SH_CHECK(CurrentStatData != nullptr, 0.0f);
	return CurrentStatData->Attack;
}

// 몬스터 경험치 반환
int32 USH_CharacterStatComponent::GetDropExp() const
{
	// 경험치 반환
	return CurrentStatData->DropExp;
}
