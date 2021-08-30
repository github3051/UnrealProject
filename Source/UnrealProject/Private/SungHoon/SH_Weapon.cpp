// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_Weapon.h"

// Sets default values
ASH_Weapon::ASH_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // ���� �ʿ����

	// ������Ʈ ����
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SH_WEAPON"));
	// root component ����
	RootComponent = Weapon;
	// �ּ� ���� ��������
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SH_ITEM_WEAPON(TEXT(
		"/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_Fencer/SK_Blade_Fencer.SK_Blade_Fencer"
	));

	if (SH_ITEM_WEAPON.Succeeded())
	{
		// �޽� ���
		Weapon->SetSkeletalMesh(SH_ITEM_WEAPON.Object);
	}
	// �浹üũ ����
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	// �⺻ ���� ���� ���� 150cm
	AttackRange = 150.0f;
	// ���� ���� �Ӽ��� �ʱ�ȭ
	AttackDamageMin = -2.5f;
	AttackDamageMax = 10.0f;
	AttackModifierMin = 0.85f;
	AttackModifierMax = 1.25f;
}

// getter
float ASH_Weapon::GetAttackRange() const
{
	return AttackRange;
}

float ASH_Weapon::GetAttackDamage() const
{
	return AttackDamage;
}

float ASH_Weapon::GetAttackModifier() const
{
	return AttackModifier;
}

// Called when the game starts or when spawned
void ASH_Weapon::BeginPlay()
{
	Super::BeginPlay();

	// �������� ������ ������ ������. (-2.5 ~ 10.0)
	AttackDamage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	AttackModifier = FMath::RandRange(AttackModifierMin, AttackModifierMax);
	
	SH_LOG(Warning, TEXT("Weapon Damage : %f, Modifier : %f"), AttackDamage, AttackModifier);
}
