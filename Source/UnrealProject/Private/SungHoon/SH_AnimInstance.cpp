// Fill out your copyright notice in the Description page of Project Settings.


#include "SungHoon/SH_AnimInstance.h"

USH_AnimInstance::USH_AnimInstance()
{
	CurrentPawnSpeed = 0.0f; // 처음에는 속도 0
	IsInAir = false; // 처음에는 땅에서 시작하므로.
}

void USH_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		// Character 클래스의 GetMovementComponent를 가져오기 위해서 억지로 캐스팅을 한거임.
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}

}
