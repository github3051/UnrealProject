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

	// ��� ������Ʈ���� �ʱ�ȭ�� ���Ŀ� ȣ��Ǵ� �Լ�
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
// ĳ���� Ŭ���������� SpringArm, Camera component�� ���� ���������Ѵ�.
	UPROPERTY(VisibleAnywhere, Category = CAMERA)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = CAMERA)
	UCameraComponent* Camera;

protected:

private:
	// ���漱��. �ִ� �ν��Ͻ��� �ʹ� ���� ����ؼ� �ƿ� ������ ����.
	UPROPERTY()
	class USH_AnimInstance* Anim;


	UPROPERTY(VisibleAnywhere, Category = Attack)
	bool bIsAttacking;

	// ���� �޺�
	UPROPERTY(VisibleAnywhere, Category = Attack)
	int32 CurrentCombo;
	// �ִ� �޺�
	UPROPERTY(VisibleAnywhere, Category = Attack)
	int32 MaxCombo;
	// ���� �޺� ������ �־�����
	UPROPERTY(VisibleAnywhere, Category = Attack)
	bool bNextComboInput;
	// ���� �޺� ������ ��������
	UPROPERTY(VisibleAnywhere, Category = Attack)
	bool bCanCombo;

};
