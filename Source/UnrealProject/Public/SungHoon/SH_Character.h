// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal.h
#include "GameFramework/Character.h"
#include "SH_Character.generated.h"

// 공격이 끝나는지를 알려주는 멀티캐스트 델리게이트 선언
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UENUM()
enum class EControlMode
{
	GTA,
	DIABLO,
	NPC
};

UCLASS()
class UNREALPROJECT_API ASH_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASH_Character();

	void SetCharacterState(ESH_CharacterState NewState);
	ESH_CharacterState GetCharacterState() const;

	// 경험치 획득량 반환 함수
	int32 GetExp() const;

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

	/*--------------------------------
				Camera
	---------------------------------*/
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

	////////////////////////////////

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;
	
	// AActor 부모 클래스의 함수를 오버라이딩함.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 폰이 빙의 됐을때 자동 호출되는 함수.
	virtual void PossessedBy(AController* NewController) override;


	/*--------------------------------
				Camera
	---------------------------------*/
	// 캐릭터 클래스에서도 SpringArm, Camera component는 직접 만들어줘야한다.
	UPROPERTY(VisibleAnywhere, Category = CAMERA)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = CAMERA)
	UCameraComponent* Camera;
	
	/*--------------------------------
		캐릭터 무기 관련 함수 및 변수
	---------------------------------*/
	// 무기를 들 수 있는지
	bool CanSetWeapon();
	// 무기 장착함수
	void SetWeapon(class ASH_Weapon* NewWeapon);
	// 무기 관련 클래스 변수
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ASH_Weapon* CurrentWeapon;

	// 전방선언 및 클래스 포인터 변수 (헤더 추가 안해도됨. 있다고만 알림)
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class USH_CharacterStatComponent* CharacterStat;

	// UI 위젯을 위한 클래스 전방선언.
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	/*--------------------------------
				Attack
	---------------------------------*/
	// 공격했을때 함수.
	// for attack animation montage
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;


	// private 멤버 함수
private:
	// for movement
	void UpDown(const float NewAxisValue);
	void LeftRight(const float NewAxisValue);

	// for rotation
	void LookUp(const float NewAxisValue);
	void Turn(const float NewAxisValue);

	// for View control mode
	void ViewChange();

	// for delegate for OnAttackMontage (몽타주가 끝나면 수행)
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// for AttackComboState
	void AttackStartComboState();
	void AttackEndComboState();

	// for Collision by attack
	void AttackCheck();

	// for Asset load
	void OnAssetLoadCompleted();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	/*--------------------------------
				Attack Combo
	---------------------------------*/
	// 다음 콤보로 이동 가능 여부
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;
	// 콤보 공격 입력 여부
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;
	// 현재 콤보 단계(0~4)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;
	// 최대 콤보 단계(4)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	////////////////////////////////

	// 전방선언. 애님 인스턴스를 너무 자주 사용해서 아예 변수로 만듦.
	UPROPERTY()
	class USH_AnimInstance* SHAnim;

	// for Debug Drawing
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;


	// FSoftObjectPath 경로명을 nullptr로 일단 지정
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	// FStreamableHandle 구조체 shared ptr로 전방선언.
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;


	// 사용중인 애셋 인덱스
	int32 AssetIndex = 0;
	// 현재 상태
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ESH_CharacterState CurrentState;
	// 플레이어냐
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;
	// AI 컨트롤러
	UPROPERTY()
	class ASH_AIController* SHAIController;
	// 플레이어 컨트롤러
	UPROPERTY()
	class ASH_PlayerController* SHPlayerController;


	// 죽음 상태 지연 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;
	// 죽음 관련 타이머 핸들
	FTimerHandle DeadTimerHandle = {};
};
