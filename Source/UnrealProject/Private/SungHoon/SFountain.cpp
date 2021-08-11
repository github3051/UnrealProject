// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SFountain.h"

// Sets default values
ASFountain::ASFountain()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SBODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SWATER"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("SLIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SSPLASH"));

	Movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("SMOVEMENT"));

	RootComponent = Body;
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	// 애셋 포인터 찾기.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));

	if (SM_BODY.Succeeded())
	{
		Body->SetStaticMesh(SM_BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WATER(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));

	if (SM_WATER.Succeeded())
	{
		Water->SetStaticMesh(SM_WATER.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_SPLASH(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01"));

	if (PS_SPLASH.Succeeded())
	{
		Splash->SetTemplate(PS_SPLASH.Object);
	}

	// test log
	LOG_TEST();

	RotateSpeed = 30.0f;
	Movement->RotationRate = FRotator(0.0f, RotateSpeed, 0.0f);
}

// Called when the game starts or when spawned
void ASFountain::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LOG_SH, Warning, TEXT("%s BeginPlay"), *GetName());

	SH_LOG_S(Error);
	SH_LOG_S(Warning);
	SH_LOG(Warning, TEXT("%s BeginPlay2"), *GetName());
}

void ASFountain::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 총 경과 시간
	SH_LOG(Error, TEXT("End Game time : %f"), GetWorld()->GetUnpausedTimeSeconds());
}

// Called every frame
void ASFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// local 기준 Z축 회전. Pitch, Yaw, Roll
	AddActorLocalRotation(FRotator(0.0f, DeltaTime*RotateSpeed, 0.0f));

	// 한 프레임 시간
	// SH_LOG(Error, TEXT("frame time : %f"), GetWorld()->GetDeltaSeconds());

	// 현재 누적된 게임 플레이 시간
	// SH_LOG(Error, TEXT("current Play sum time : %f"), GetWorld()->GetTimeSeconds());
}

void ASFountain::LOG_TEST()
{
	SH_LOG(Warning, TEXT("%s Constructor"), *GetName());
}