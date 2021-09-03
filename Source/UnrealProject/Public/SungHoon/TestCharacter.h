// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h"
#include "GameFramework/Character.h"
#include "TestCharacter.generated.h"

UCLASS()
class UNREALPROJECT_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 모든 컴포넌트들이 초기화된 이후에 호출되는 함수
	virtual void PostInitializeComponents() override;

private:
	// for movement
	void UpDown(const float NewAxisValue);
	void LeftRight(const float NewAxisValue);

	// for rotation
	void LookUp(const float NewAxisValue);
	void Turn(const float NewAxisValue);

	// for attack animation montage
	void TestAttack();
	
	UFUNCTION()
	void OnTestAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
////////////////////////////////////////////////////////////////////////////

public:
	/*--------------------------------
			Camera
	---------------------------------*/
// 캐릭터 클래스에서도 SpringArm, Camera component는 직접 만들어줘야한다.
	UPROPERTY(VisibleAnywhere, Category = CAMERA)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = CAMERA)
	UCameraComponent* Camera;

protected:

private:
	// 전방선언. 애님 인스턴스를 너무 자주 사용해서 아예 변수로 만듦.
	UPROPERTY()
	class USH_AnimInstance* Anim;


	UPROPERTY(VisibleAnywhere, Category = Attack)
	bool bIsAttacking;

	// 현재 콤보
	UPROPERTY(VisibleAnywhere, Category = Attack)
	int32 CurrentCombo;
	// 최대 콤보
	UPROPERTY(VisibleAnywhere, Category = Attack)
	int32 MaxCombo;
	// 다음 콤보 공격이 있었는지
	UPROPERTY(VisibleAnywhere, Category = Attack)
	bool bNextComboInput;
	// 다음 콤보 공격을 실행할지
	UPROPERTY(VisibleAnywhere, Category = Attack)
	bool bCanCombo;

};
