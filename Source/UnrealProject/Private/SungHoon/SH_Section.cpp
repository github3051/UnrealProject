// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_Section.h"
#include "SungHoon/SH_Character.h"
#include "SungHoon/SH_ItemBox.h"

// Sets default values
ASH_Section::ASH_Section()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // off
	SH_LOG_S(Warning);
	/*------------
		SQUARE
	-------------*/
	// 컴포넌트 생성
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	// RootComponent 설정
	RootComponent = Mesh;

	FString AssetPath = TEXT("/Game/PublicAsset/Chapter1/Book/StaticMesh/SM_SQUARE.SM_SQUARE");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SH_SQUARE(*AssetPath);

	if (SH_SQUARE.Succeeded())
	{
		// 애셋 설정
		Mesh->SetStaticMesh(SH_SQUARE.Object);
	}
	else
	{
		SH_LOG(Error, TEXT("Failed to load StaticMesh asset. : %s"), *AssetPath);
	}

	/*-------------------
		  BoxTrigger
	---------------------*/
	// 트리거로 사용할 박스 컴포넌트 생성
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	// 박스 사이즈 늘림
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	// 컴포넌트 부착 (-> SM_SQUARE 메시의 정중앙에 배치되게 될거임)
	Trigger->SetupAttachment(RootComponent);
	// 위치 조정
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	// 콜리전 채널 설정
	Trigger->SetCollisionProfileName(TEXT("SH_Trigger"));
	// BoxComponent에 있는 델리게이트
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASH_Section::OnTriggerBeginOverlap);

	/*------------
		 GATE
	-------------*/
	FString GateAssetPath = TEXT("/Game/PublicAsset/Chapter1/Book/StaticMesh/SM_GATE.SM_GATE");
	// 애셋 가져오기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SH_GATE(*GateAssetPath);
	// 애셋을 가져오지 못했으면
	if (!SH_GATE.Succeeded())
	{
		SH_LOG(Error, TEXT("Failed to load StaticMesh asset. : %s"), *GateAssetPath);
	}

	static FName GateSockets[] = { {TEXT("+XGate")}, {TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate") } };

	// 해당하는 소켓명을 하나씩 돌면서
	for (const FName& GateSocket : GateSockets)
	{
		// 해당하는 소켓명을 가지고 있는지 체크. 있으면 통과
		SH_CHECK(Mesh->DoesSocketExist(GateSocket));
		// 스태텍 메시 컴포넌트 생성. 소켓명을 가지고
		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
		// 애셋 적용
		NewGate->SetStaticMesh(SH_GATE.Object);
		// GateSocket명에 해당하는 소켓에 부착
		NewGate->SetupAttachment(RootComponent, GateSocket);
		// 피봇 기준으로 -80.5 만큼 왼쪽으로 이동 (y축)
		NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		// 배열에 넣음
		GateMeshes.Add(NewGate);

		// 각 게이트마다 트리거 박스 생성
		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));
		// 생성한 박스 컴포넌트 크기 조절
		NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		// 컴포넌트 부착, GateSocket에 해당하는 곳에 부착됨. 게이트 생성 위치와 같음
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		// 컴포넌트 위치 조절
		NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		// 컴포넌트 콜리전 채널 설정
		NewGateTrigger->SetCollisionProfileName(TEXT("SH_Trigger"));
		// Trigger를 모아두는 배열에 추가함.
		GateTriggers.Add(NewGateTrigger);

		// 각 게이트 트리거마다 델리게이트 함수 바인딩함.
		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASH_Section::OnGateTriggerBeginOverlap);
		// 각 트리거마다 컴포넌트의 태그에 본인 소켓명을 설정함. (구별짓기 위함)
		NewGateTrigger->ComponentTags.Add(GateSocket);

	}

	// State 초기화
	bNoBattle = false;

	// 시간 초기화
	EnemySpawnTime = 2.0f;
	ItemBoxSpawnTime = 4.0f;
}

// 에디터 작업에서 선택한 액터의 속성이나 트랜스폼 정보가 변결될때 자동 호출되는 함수
void ASH_Section::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// LOP 찍도록함.
	SH_LOG(Warning, TEXT("Construction! : %s"), *GetName());
	// bNoBattle이 참이면 COMPLETE부터
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called when the game starts or when spawned
void ASH_Section::BeginPlay()
{
	Super::BeginPlay();
	SH_LOG(Warning, TEXT("BeginPlay! : %s"), *GetName());
	// bNoBattle이 참이면 COMPLETE부터
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void ASH_Section::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	case ESectionState::READY:
		/*	TODO
			문을 열어놓고 대기하다가,
			중앙의 제일 큰 트리거 박스로 진입하면 다음 스테이지로
		*/
		SH_LOG(Error,TEXT("Start READY state : %s"), *GetName());
		// 중앙의 제일 큰 트리거 박스 콜리전 채널 ON. 작동
		Trigger->SetCollisionProfileName(TEXT("SH_Trigger"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			// 문에 있는 트리거 박스 충돌 체크 끔
			GateTrigger->SetCollisionProfileName("NoCollision");
		}
		// 문을 연다.
		OperateGates(true);
		break;
		
	case ESectionState::BATTLE:
		/*	TODO
			문을 닫고 일정 시간이 지나면 NPC와 아이템 상자를 소환.
			NPC가 죽으면 COMPLETE로 이동
		*/
		// 가장 큰 트리거 박스 콜리전 채널 끔.
		SH_LOG(Error, TEXT("Start BATTLE state : %s"), *GetName());
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			// 문에 있는 트리거 박스 충돌 체크 끔
			GateTrigger->SetCollisionProfileName("NoCollision");
		}
		// 문을 닫음
		OperateGates(false);

		// 타이머를 생성하고, EnemySpawnTime 시간후에 델리게이트 함수를 호출함. OnNPCSpawn. 1번만 실행되겠지.
		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle, FTimerDelegate::CreateUObject(
			this, &ASH_Section::OnNPCSpawn), EnemySpawnTime, false);

		// 타이머를 생성하고, ItemBoxSpawnTime 시간후에 델리게이트 람다함수를 호출함. 1번만 실행되겠지.
		GetWorld()->GetTimerManager().SetTimer(SpawnItemBoxTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			
			// 600cm 원의 랜덤한 좌표값
			FVector2D RandXY = FMath::RandPointInCircle(600.0f);
			// 아이템을 생성시킴. 맵 중앙 위치값 + 랜덤한 반경 600cm = 6m 위치로.
			GetWorld()->SpawnActor<ASH_ItemBox>(GetActorLocation() + FVector(RandXY, 60.0f), FRotator::ZeroRotator);

		}), ItemBoxSpawnTime, false);

		break;

	case ESectionState::COMPLETE:
		/*	TODO
			닫힌 문을 열고, 각 문마다 배치한 Trigger 게이트로
			플레이를 감지하면 이동한 문의 방향으로 새로운 섹션 소환.
		*/
		// 가장 큰 트리거 박스 콜리전 채널 끔.
		SH_LOG(Error, TEXT("Start COMPLETE state : %s"), *GetName());
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			// 문에 있는 트리거 박스 충돌 체크 켬.
			GateTrigger->SetCollisionProfileName("SH_Trigger");
		}
		// 문을 열고
		OperateGates(true);
		break;
	default:
		break;
	}

	// 현재 state 업데이트
	CurrentState = NewState;
}

void ASH_Section::OperateGates(bool bOpen)
{
	// 문에 대한 메시를 돌면서
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		// 문을 여는거면 회전시키고, 아니면 원본 위치로 리셋.
		Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}

// 중앙의 큰 박스
void ASH_Section::OnTriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	SH_LOG_S(Warning);
	// 만약 현재 섹션 state가 READY라면
	if (CurrentState == ESectionState::READY)
	{
		// 다음 섹션으로 이동
		SetState(ESectionState::BATTLE);
	}
}

// 각 문에 있는 트리거 박스
void ASH_Section::OnGateTriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	SH_LOG_S(Warning);
	// 해당 컴포넌트의 태그가 1개만 있는지 체크. 맞다면 통과
	SH_CHECK(OverlappedComponent->ComponentTags.Num() == 1);
	// 해당 태그명을 가져옴
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	// 태그의 문자열 왼쪽 2개를 가져옴. (+x, -x, +y, -y)
	FName SocketName = FName(*ComponentTag.ToString().Left(2));

	// 만약 소켓 이름에 해당하는것이 메시에 없다면 종료.
	if (!Mesh->DoesSocketExist(SocketName))
	{
		return;
	}
	
	// 소켓명에 해당하는 위치값 저장
	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	// 충돌 결과를 담을 변수
	TArray<FOverlapResult> OverlapResults;
	// 조건식 변수들 설정
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);
	
	// 생성시키려는 위치로 충돌체크를 해봄.
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults, // 결과를 담음
		NewLocation, // 충돌체크를 시작하는 중심점
		FQuat::Identity, // 회전값
		ObjectQueryParam, // 모든 Object 검사
		FCollisionShape::MakeSphere(775.0f), // 구체
		CollisionQueryParam // 현재 자신 섹션 맵 타일은 제외
	);

	// 충돌된게 없다면
	if (!bResult)
	{
		// 새로운 섹션을 생성시켜준다.
		auto NewSection = GetWorld()->SpawnActor<ASH_Section>(NewLocation, FRotator::ZeroRotator);
	}
	// 충돌된게 있다면, 이미 만들어진 섹션이 존재한다느 의미.
	else
	{
		SH_LOG(Warning, TEXT("New section area is not empty. already existed"));
	}
}

// AI 생성 함수
void ASH_Section::OnNPCSpawn()
{
	// 현재 맵의 중앙 위치값 + 88.0f 높이 위로, 회전값은 기본값으로 액터를생성시킨다.
	GetWorld()->SpawnActor<ASH_Character>(GetActorLocation() + FVector::UpVector*88.0f, FRotator::ZeroRotator);
}

