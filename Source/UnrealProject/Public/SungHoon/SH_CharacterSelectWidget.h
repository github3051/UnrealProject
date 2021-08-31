// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "Blueprint/UserWidget.h"
#include "SH_CharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USH_CharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	// �������Ʈ������ ȣ���� �� �ֵ��� ����
	UFUNCTION(BlueprintCallable)
	void NextCharacter(bool bForward = true);

	// UI�� ��� �غ� �Ϸ�Ǹ� �ѹ� ȣ��Ǵ� �Լ�.
	virtual void NativeConstruct() override;

private:
	// ��ư ���ε� �Լ�
	UFUNCTION()
	void OnPrevClicked();
	UFUNCTION()
	void OnNextClicked();
	UFUNCTION()
	void OnConfirmClicked();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 CurrentIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 MaxIndex;

	// WeakPtr ����
	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;

	// �� ��ư�� �ش��ϴ� ������ ���� ���漱��
	UPROPERTY()
	class UButton* PrevButton;
	UPROPERTY()
	class UButton* NextButton;
	UPROPERTY()
	class UEditableTextBox* TextBox;
	UPROPERTY()
	class UButton* ConfirmButton;

};
