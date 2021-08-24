// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_ItemBox.h"
#include "SungHoon/SHWeapon.h" 
#include "SungHoon/SH_Character.h"

// Sets default values
ASH_ItemBox::ASH_ItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // tick을 꺼주자.

	// 컴포넌트 생성
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("SH_TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SH_BOX"));
	
	// 루트 컴포넌트 설정, 기타 컴포넌트 부착
	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);

	// 박스 컴포넌트 크기 조절
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	Box->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));

	// 스태틱 메시에 사용할 애셋 정보 가져오기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SH_ITEM_BOX(TEXT(
		"/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"
	));
	if (SH_ITEM_BOX.Succeeded())
	{
		// 스태틱 메시 컴포넌트에 메시 애셋 적용
		Box->SetStaticMesh(SH_ITEM_BOX.Object);
	}
	
	// 박스 컴포넌트의 최초 위치값 설정, 하지만 월드 배치가 최종적인 위치값이 된다.
	Trigger->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	// 채널 설정
	Trigger->SetCollisionProfileName(TEXT("SH_ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	// SHWeapon 클래스 정보를 가져옴. 헤더파일 추가해야함.
	WeaponItemClass = ASHWeapon::StaticClass();

	/*-------------
		Effect
	--------------*/
	// 파티클 컴포넌트 생성
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SH_EFFECT"));
	// 컴포넌트 부착
	Effect->SetupAttachment(RootComponent);

	// 컴포넌트에 넣을 애셋 정보 가져오기
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SH_CHESTOPEN(TEXT(
		"/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"
	));
	if (SH_CHESTOPEN.Succeeded())
	{
		// 파티클 애셋 지정
		Effect->SetTemplate(SH_CHESTOPEN.Object); // effect는 template이다
		// 자동 활성화는 끈다.
		Effect->bAutoActivate = false;
	}
}

// Called when the game starts or when spawned
void ASH_ItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASH_ItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASH_ItemBox::OnCharacterOverlap);
}

// Called every frame
void ASH_ItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASH_ItemBox::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// 겹치면 로그만 띄우도록 해보자.
	SH_LOG_S(Error);

	// 겹친 상대방 액터를 가져와서 캐스팅
	auto SHCharacter = Cast<ASH_Character>(OtherActor);
	// 정상적으로 캐스팅 됐다면 통과
	SH_CHECK(SHCharacter != nullptr);

	// 무기 클래스 정보를 제대로 갖고 있다면
	if (WeaponItemClass != nullptr && SHCharacter != nullptr)
	{
		// 캐릭터가 무기를 가질 수 있다면 (무기가 아직 없다면)
		if (SHCharacter->CanSetWeapon())
		{
			// 무기 액터를 생성함.
			auto NewWeapon = GetWorld()->SpawnActor<ASHWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			// 생성한 무기 액터를 캐릭터에 장착
			SHCharacter->SetWeapon(NewWeapon);

			// 이펙트 활성화
			Effect->Activate(true);
			// 스태틱 메시 감춤. 상속받은것도 전부
			Box->SetHiddenInGame(true, true);
			// 아이템 박스 액터에 대한 충돌도 끄기 -> 이벤트 발생 막음
			SetActorEnableCollision(false);
			// 델리게이트 등록. OnEffectFinished을 실행시킴.
			Effect->OnSystemFinished.AddDynamic(this, &ASH_ItemBox::OnEffectFinished);
		}
		// 무기를 이미 가지고 있어서 더 가질 수 없다면
		else
		{
			// 로그를 찍음
			SH_LOG(Error, TEXT("%s can't equip weapon currently. already had it."), *SHCharacter->GetName());
		}
	}
}

void ASH_ItemBox::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	// 지금 액터를 파괴함.
	Destroy();
}