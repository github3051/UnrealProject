// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_CharacterSelectWidget.h"
#include "SH_CharacterSetting.h" // UnrealProjectSetting 모듈에 있는거
#include "SungHoon/SH_GameInstance.h"
#include "EngineUtils.h" // for TActorIterator
#include "Animation/SkeletalMeshActor.h" // for ASkeletalMeshActor
#include "Components/Button.h" // for Button 클래스 내부 기능 사용
#include "Components/EditableTextBox.h" // for UEditableTextBox 변수 내부 기능 사용
#include "SungHoon/SH_SaveGame.h"
#include "SungHoon/SH_PlayerState.h"


void USH_CharacterSelectWidget::NextCharacter(bool bForward)
{
	// 오른쪽 버튼을 눌렀다면 ++
	bForward ? CurrentIndex++ : CurrentIndex--;

	// 미달 했다면
	if (CurrentIndex == -1)
	{
		// -1이 되면 최대값에서 1뺀값으로 이동.
		CurrentIndex = MaxIndex - 1;
	}
	// 초과했다면
	if (CurrentIndex == MaxIndex)
	{
		// 0으로
		CurrentIndex = 0;
	}

	// 애셋 정보가 있는 CharacterSetting 클래스 정보 가져옴
	auto CharacterSetting = GetDefault<USH_CharacterSetting>();
	SH_CHECK(CharacterSetting != nullptr);
	// CurrentIndex에 해당하는 애셋 정보의 경로를 가져옴. 경로변수.
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto SHGameInstance = GetWorld()->GetGameInstance<USH_GameInstance>();
	SH_CHECK(SHGameInstance != nullptr);
	SH_CHECK(TargetComponent.IsValid());

	// 해당하는 경로의 애셋을 게임 실행중에 동기화 방식으로 가져옴
	USkeletalMesh* Asset = SHGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);

	// 잘 가져왔다면
	if (Asset != nullptr)
	{
		// 컴포넌트에 스켈레탈 메시 적용
		TargetComponent->SetSkeletalMesh(Asset);
	}
}

void USH_CharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	// 애셋 정보가 담긴 클래스 가져오기
	auto CharacterSetting = GetDefault<USH_CharacterSetting>();

	// 해당 애셋 개수를 가져옴
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	// 현재 월드에 있는 ASkeletalMeshActor을 상속받은 액터의 목록을 가져옴.
	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		// 결국 현재 레벨에 지정한 메시를 가져와 등록한다는 셈. 시작 메시
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	SH_CHECK(PrevButton != nullptr);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	SH_CHECK(NextButton != nullptr);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	SH_CHECK(ConfirmButton != nullptr);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	SH_CHECK(TextBox != nullptr);

	// 델리게이트 바인딩
	PrevButton->OnClicked.AddDynamic(this, &USH_CharacterSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &USH_CharacterSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &USH_CharacterSelectWidget::OnConfirmClicked);
}


// 왼쪽 버튼
void USH_CharacterSelectWidget::OnPrevClicked()
{
	// false로 날림
	NextCharacter(false);
}

// 오른쪽 버튼
void USH_CharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

// 캐릭터 이름명 확인 버튼
void USH_CharacterSelectWidget::OnConfirmClicked()
{
	// 텍스트 박스에 있는것을 String으로 가져옴
	FString CharacterName = TextBox->GetText().ToString();

	// 캐릭터 이름이 없거나, 10보다 크면 (10칸까지 가능)
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10)
	{
		// 종료
		return;
	}

	// 게임 세이브 데이터 만들기
	USH_SaveGame* NewPlayerData = NewObject<USH_SaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	// 플레이어 스테이트
	auto SHPlayerState = GetDefault<ASH_PlayerState>();
	
	// 성공적으로 게임 데이터를 저장하였다면 (세이브데이터, 저장할 슬롯명, 슬롯 인덱스)
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, SHPlayerState->SaveSlotName, 0))
	{
		// SH_GamePlay라는 레벨을 열어라
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("SH_Gameplay"));
	}
	// 슬롯에 저장하지 못했다면
	else
	{
		SH_LOG(Error, TEXT("SaveGame Error!"));
	}
}
