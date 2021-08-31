// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_CharacterSelectWidget.h"
#include "SH_CharacterSetting.h" // UnrealProjectSetting ��⿡ �ִ°�
#include "SungHoon/SH_GameInstance.h"
#include "EngineUtils.h" // for TActorIterator
#include "Animation/SkeletalMeshActor.h" // for ASkeletalMeshActor
#include "Components/Button.h" // for Button Ŭ���� ���� ��� ���
#include "Components/EditableTextBox.h" // for UEditableTextBox ���� ���� ��� ���
#include "SungHoon/SH_SaveGame.h"
#include "SungHoon/SH_PlayerState.h"


void USH_CharacterSelectWidget::NextCharacter(bool bForward)
{
	// ������ ��ư�� �����ٸ� ++
	bForward ? CurrentIndex++ : CurrentIndex--;

	// �̴� �ߴٸ�
	if (CurrentIndex == -1)
	{
		// -1�� �Ǹ� �ִ밪���� 1�������� �̵�.
		CurrentIndex = MaxIndex - 1;
	}
	// �ʰ��ߴٸ�
	if (CurrentIndex == MaxIndex)
	{
		// 0����
		CurrentIndex = 0;
	}

	// �ּ� ������ �ִ� CharacterSetting Ŭ���� ���� ������
	auto CharacterSetting = GetDefault<USH_CharacterSetting>();
	SH_CHECK(CharacterSetting != nullptr);
	// CurrentIndex�� �ش��ϴ� �ּ� ������ ��θ� ������. ��κ���.
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto SHGameInstance = GetWorld()->GetGameInstance<USH_GameInstance>();
	SH_CHECK(SHGameInstance != nullptr);
	SH_CHECK(TargetComponent.IsValid());

	// �ش��ϴ� ����� �ּ��� ���� �����߿� ����ȭ ������� ������
	USkeletalMesh* Asset = SHGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);

	// �� �����Դٸ�
	if (Asset != nullptr)
	{
		// ������Ʈ�� ���̷�Ż �޽� ����
		TargetComponent->SetSkeletalMesh(Asset);
	}
}

void USH_CharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	// �ּ� ������ ��� Ŭ���� ��������
	auto CharacterSetting = GetDefault<USH_CharacterSetting>();

	// �ش� �ּ� ������ ������
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	// ���� ���忡 �ִ� ASkeletalMeshActor�� ��ӹ��� ������ ����� ������.
	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		// �ᱹ ���� ������ ������ �޽ø� ������ ����Ѵٴ� ��. ���� �޽�
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

	// ��������Ʈ ���ε�
	PrevButton->OnClicked.AddDynamic(this, &USH_CharacterSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &USH_CharacterSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &USH_CharacterSelectWidget::OnConfirmClicked);
}


// ���� ��ư
void USH_CharacterSelectWidget::OnPrevClicked()
{
	// false�� ����
	NextCharacter(false);
}

// ������ ��ư
void USH_CharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

// ĳ���� �̸��� Ȯ�� ��ư
void USH_CharacterSelectWidget::OnConfirmClicked()
{
	// �ؽ�Ʈ �ڽ��� �ִ°��� String���� ������
	FString CharacterName = TextBox->GetText().ToString();

	// ĳ���� �̸��� ���ų�, 10���� ũ�� (10ĭ���� ����)
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10)
	{
		// ����
		return;
	}

	// ���� ���̺� ������ �����
	USH_SaveGame* NewPlayerData = NewObject<USH_SaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	// �÷��̾� ������Ʈ
	auto SHPlayerState = GetDefault<ASH_PlayerState>();
	
	// ���������� ���� �����͸� �����Ͽ��ٸ� (���̺굥����, ������ ���Ը�, ���� �ε���)
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, SHPlayerState->SaveSlotName, 0))
	{
		// SH_GamePlay��� ������ �����
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("SH_Gameplay"));
	}
	// ���Կ� �������� ���ߴٸ�
	else
	{
		SH_LOG(Error, TEXT("SaveGame Error!"));
	}
}
