// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal.h
#include "GameFramework/Character.h"
#include "SH_Character.generated.h"


UENUM()
enum class EControlMode
{
	GTA,
	DIABLO
};

UCLASS()
class UNREALPROJECT_API ASH_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASH_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// View 작동방식 설정함수. GTA vs DIABLO
	void SetControlMode(const EControlMode& NewControlMode);

	// 초기 컨트롤 모드값 설정
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	EControlMode CurrentControlMode;
	
	// 움직이는 방향쪽 방향벡터
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	FVector DirectionToMove;

	// for SpringArm's InterpTo values. 화면전환에 쓰일 변수
	// 카메라까지 거리
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	float ArmLengthTo = 0.0f;
	// 카메라의 각도
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	// 길이 조절 속도
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	float ArmLengthSpeed = 0.0f;
	// 카메라 손잡이가 회전하는 속도
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	float ArmRotationSpeed = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 캐릭터 클래스에서도 SpringArm, Camera component는 직접 만들어줘야한다.
	UPROPERTY(VisibleAnywhere, Category = CAMERA)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = CAMERA)
	UCameraComponent* Camera;

private:
	// for movement
	void UpDown(const float NewAxisValue);
	void LeftRight(const float NewAxisValue);

	// for rotation
	void LookUp(const float NewAxisValue);
	void Turn(const float NewAxisValue);

	// for View control mode
	void ViewChange();

};
