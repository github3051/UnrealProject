// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SHPawn.h"

// Sets default values
ASHPawn::ASHPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SHCAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SHMESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("SHMOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SHSPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("SHCAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);

	// 한번에 저장가능
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// 스켈레탈 메시 애셋 찾아오기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));

	// 메시 애셋을 찾아왔으면
	if (SK_CARDBOARD.Succeeded())
	{
		// 실제 메시에 애셋을 등록. 화면에 출력됨.
		Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// 애니메이션 모드 설정. 애니메이션 블루프린트
	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	// 애님인스턴스 파일을 찾아옴.
	static ConstructorHelpers::FClassFinder<UAnimInstance> SH_WARRIOR_ANIM(TEXT("/Game/SungHoon/Animation/SH_WarriorAnimBlueprint.SH_WarriorAnimBlueprint_C"));

	// 애님 인스턴스를 잘 가져왔다면
	if (SH_WARRIOR_ANIM.Succeeded())
	{
		// 메시에 있는 애님 인스턴스 클래스에 워리어 애님 인스턴스를 등록.
		Mesh->SetAnimInstanceClass(SH_WARRIOR_ANIM.Class);
	}


}

// Called when the game starts or when spawned
void ASHPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// 이거 뭔지 공부하기.
	//Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/PublicAsset/Chapter5/WarriorRun.WarriorRun"));

	//if (AnimAsset != nullptr)
	//{
	//	// 애니메이션 실행은 매쉬에서 가능.
	//	Mesh->PlayAnimation(AnimAsset, true);
	//}
}

// Called every frame
void ASHPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASHPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SH_LOG_S(Warning);
}

void ASHPawn::PossessedBy(AController* NewController)
{
	SH_LOG_S(Warning);
	Super::PossessedBy(NewController);
}

// Called to bind functionality to input
void ASHPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown_SH"), this, &ASHPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight_SH"), this, &ASHPawn::LeftRight);
}

void ASHPawn::UpDown(float NewAxisValue)
{
	SH_LOG(Warning, TEXT("%f"), NewAxisValue);
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void ASHPawn::LeftRight(float NewAxisValue)
{
	SH_LOG(Warning, TEXT("%f"), NewAxisValue);
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}
