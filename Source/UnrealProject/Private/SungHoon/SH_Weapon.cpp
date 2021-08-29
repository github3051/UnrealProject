// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_Weapon.h"

// Sets default values
ASH_Weapon::ASH_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // 굳이 필요없음

	// 컴포넌트 생성
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SH_WEAPON"));
	// root component 설정
	RootComponent = Weapon;
	// 애셋 정보 가져오기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SH_ITEM_WEAPON(TEXT(
		"/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_Fencer/SK_Blade_Fencer.SK_Blade_Fencer"
	));

	if (SH_ITEM_WEAPON.Succeeded())
	{
		// 메시 등록
		Weapon->SetSkeletalMesh(SH_ITEM_WEAPON.Object);
	}
	// 충돌체크 끄기
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ASH_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}
