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
	// 블루프린트에서도 호출할 수 있도록 설정
	UFUNCTION(BlueprintCallable)
	void NextCharacter(bool bForward = true);

	// UI가 모두 준비 완료되면 한번 호출되는 함수.
	virtual void NativeConstruct() override;

private:
	// 버튼 바인딩 함수
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

	// WeakPtr 선언
	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;

	// 각 버튼에 해당하는 변수에 대한 전방선언
	UPROPERTY()
	class UButton* PrevButton;
	UPROPERTY()
	class UButton* NextButton;
	UPROPERTY()
	class UEditableTextBox* TextBox;
	UPROPERTY()
	class UButton* ConfirmButton;

};
