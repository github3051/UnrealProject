// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_CharacterWidget.h"
#include "SungHoon/SH_CharacterStatComponent.h"
#include "Components/ProgressBar.h"




void USH_CharacterWidget::BindCharacterStat(class USH_CharacterStatComponent* NewCharacterStat)
{
	// NewCharacterStat이 nullptr 아니면 통과
	SH_CHECK(NewCharacterStat != nullptr);

	// Character 클래스로부터 CharacterStat 정보를 받아옴.
	CurrentCharacterStat = NewCharacterStat;
	
	// 델리게이트 함수 등록. 델리게이트 선언부는 SH_CharacterStatComponent에 있음. 여기는 외부.
	NewCharacterStat->OnHPChanged.AddUObject(this, &USH_CharacterWidget::UpdateHPWidget);
}

void USH_CharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯의 이름을 통해 가져옴. 캐스팅
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	// HPProgressBar 값이 유효하다면
	SH_CHECK(HPProgressBar != nullptr);

	UpdateHPWidget();
}

void USH_CharacterWidget::UpdateHPWidget()
{
	// CurrentCharacterStat 유효하다면
	// NativeConstruct 호출되기전에 BindCharacterStat이 호출
	// BindCharacterStat는 Character의 BeginPlay에서 호출됨.
	if (CurrentCharacterStat.IsValid())
	{
		if (HPProgressBar != nullptr)
		{
			// 현재 HP 비율의 소숫점 float을 받아와서 PRogressBar에 적용
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
