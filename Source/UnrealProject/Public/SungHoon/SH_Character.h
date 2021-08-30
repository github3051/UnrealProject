// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal.h
#include "GameFramework/Character.h"
#include "SH_Character.generated.h"

// ������ ���������� �˷��ִ� ��Ƽĳ��Ʈ ��������Ʈ ����
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

	// ����ġ ȹ�淮 ��ȯ �Լ�
	int32 GetExp() const;

	// ���� ���� ���� �Լ�.
	float GetFinalAttackRange() const;
	// ���� ������
	float GetFinalAttackDamage() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// View �۵���� �����Լ�. GTA vs DIABLO
	void SetControlMode(const EControlMode& NewControlMode);

	// �ʱ� ��Ʈ�� ��尪 ����
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	EControlMode CurrentControlMode;
	
	// �����̴� ������ ���⺤��
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	FVector DirectionToMove;

	/*--------------------------------
				Camera
	---------------------------------*/
	// for SpringArm's InterpTo values. ȭ����ȯ�� ���� ����
	// ī�޶���� �Ÿ�
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	float ArmLengthTo = 0.0f;
	// ī�޶��� ����
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	// ���� ���� �ӵ�
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	float ArmLengthSpeed = 0.0f;
	// ī�޶� �����̰� ȸ���ϴ� �ӵ�
	UPROPERTY(VisibleAnywhere, Category = DEBUGING)
	float ArmRotationSpeed = 0.0f;

	////////////////////////////////

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//
	virtual void PostInitializeComponents() override;
	
	// AActor �θ� Ŭ������ �Լ��� �������̵���.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// ���� ���� ������ �ڵ� ȣ��Ǵ� �Լ�.
	virtual void PossessedBy(AController* NewController) override;


	/*--------------------------------
				Camera
	---------------------------------*/
	// ĳ���� Ŭ���������� SpringArm, Camera component�� ���� ���������Ѵ�.
	UPROPERTY(VisibleAnywhere, Category = CAMERA)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = CAMERA)
	UCameraComponent* Camera;
	
	/*--------------------------------
		ĳ���� ���� ���� �Լ� �� ����
	---------------------------------*/
	// ���⸦ �� �� �ִ���
	bool CanSetWeapon();
	// ���� �����Լ�
	void SetWeapon(class ASH_Weapon* NewWeapon);
	// ���� ���� Ŭ���� ����
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ASH_Weapon* CurrentWeapon;

	// ���漱�� �� Ŭ���� ������ ���� (��� �߰� ���ص���. �ִٰ� �˸�)
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class USH_CharacterStatComponent* CharacterStat;

	// UI ������ ���� Ŭ���� ���漱��.
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	/*--------------------------------
				Attack
	---------------------------------*/
	// ���������� �Լ�.
	// for attack animation montage
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;


	// private ��� �Լ�
private:
	// for movement
	void UpDown(const float NewAxisValue);
	void LeftRight(const float NewAxisValue);

	// for rotation
	void LookUp(const float NewAxisValue);
	void Turn(const float NewAxisValue);

	// for View control mode
	void ViewChange();

	// for delegate for OnAttackMontage (��Ÿ�ְ� ������ ����)
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
	// ���� �޺��� �̵� ���� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;
	// �޺� ���� �Է� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;
	// ���� �޺� �ܰ�(0~4)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;
	// �ִ� �޺� �ܰ�(4)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	////////////////////////////////

	// ���漱��. �ִ� �ν��Ͻ��� �ʹ� ���� ����ؼ� �ƿ� ������ ����.
	UPROPERTY()
	class USH_AnimInstance* SHAnim;

	// for Debug Drawing
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;


	// FSoftObjectPath ��θ��� nullptr�� �ϴ� ����
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	// FStreamableHandle ����ü shared ptr�� ���漱��.
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;


	// ������� �ּ� �ε���
	int32 AssetIndex = 0;
	// ���� ����
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ESH_CharacterState CurrentState;
	// �÷��̾��
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;
	// AI ��Ʈ�ѷ�
	UPROPERTY()
	class ASH_AIController* SHAIController;
	// �÷��̾� ��Ʈ�ѷ�
	UPROPERTY()
	class ASH_PlayerController* SHPlayerController;


	// ���� ���� ���� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;
	// ���� ���� Ÿ�̸� �ڵ�
	FTimerHandle DeadTimerHandle = {};
};
