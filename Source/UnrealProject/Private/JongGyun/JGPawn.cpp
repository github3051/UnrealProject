// Fill out your copyright notice in the Description page of Project Settings.


#include "JongGyun/JGPawn.h"

// Sets default values
AJGPawn::AJGPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>BOX(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (BOX.Succeeded())
	{
		Mesh->SetSkeletalMesh(BOX.Object);
	}

	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> BOX_Anim(TEXT("/Game/JongGyun/JGWarriorAnimBP.JGWarriorAnimBP_C"));
	if (BOX_Anim.Succeeded())
	{
		Mesh->SetAnimInstanceClass(BOX_Anim.Class);
	}

}

// Called when the game starts or when spawned
void AJGPawn::BeginPlay()
{
	Super::BeginPlay();
	//Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/PublicAsset/Chapter5/WarriorRun.WarriorRun"));
	//if (AnimAsset != nullptr)
	//{
	//	Mesh->PlayAnimation(AnimAsset, true);
	//}
	
}

// Called every frame
void AJGPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJGPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	JGLOG_S(Warning);
}

void AJGPawn::PossessedBy(AController * NewController)
{
	JGLOG_S(Warning);
	Super::PossessedBy(NewController);
	
}

// Called to bind functionality to input
void AJGPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AJGPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AJGPawn::LeftRight);

}

void AJGPawn::UpDown(float NewAxisValue)
{
	//Log
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AJGPawn::LeftRight(float NewAxisValue)
{
	//Log
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}
