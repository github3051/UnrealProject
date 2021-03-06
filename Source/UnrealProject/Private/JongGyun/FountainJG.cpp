// Fill out your copyright notice in the Description page of Project Settings.


#include "JongGyun/FountainJG.h"

// Sets default values
AFountainJG::AFountainJG()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JGBODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JGWATER"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("JGLIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("JGSPLASH"));
	Movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("MOVEMENT"));

	RootComponent = Body;
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);
	
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));	// 기본 값으로 설정
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));
	if (SM_BODY.Succeeded())
	{
		Body->SetStaticMesh(SM_BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_WATER(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));
	if (SM_WATER.Succeeded())
	{
		Water->SetStaticMesh(SM_WATER.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PS_SPLASH(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01"));
	if (PS_SPLASH.Succeeded())
	{
		Splash->SetTemplate(PS_SPLASH.Object);
	}

	RotateSpeed = 30.0f;
	Movement->RotationRate = FRotator(0.0f, RotateSpeed, 0.0f);

}

// Called when the game starts or when spawned
void AFountainJG::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LOG_JG, Warning, TEXT("Actor Name : %s, ID : %d, Location X:%.3f"), *GetName(), ID, GetActorLocation().X);
	//JGLOG_S(Warning);
	JGLOG(Warning, TEXT("Actor Name : %s, ID : %d, Location X:%.3f"), *GetName(), ID, GetActorLocation().X);
}


//void AFountainJG::EndPlay(const EEndplayReason::Type EndPlayReason)
//{
//	Super::EndPlay(EndPlayReason);
//	JGLOG_S(Warning);
//}




// Called every frame
void AFountainJG::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.0f, RotateSpeed * DeltaTime, 0.0f));
}


void AFountainJG::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	JGLOG_S(Warning);
}