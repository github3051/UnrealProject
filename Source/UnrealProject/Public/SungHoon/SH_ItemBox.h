// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "../../UnrealProject.h" // for EngineMinimal
#include "GameFramework/Actor.h"
#include "SH_ItemBox.generated.h"


UCLASS()
class UNREALPROJECT_API ASH_ItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASH_ItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 박스 컴포턴트
	UPROPERTY(VisibleAnywhere, Category = Box)
	UBoxComponent* Trigger;

	// 스태틱메시 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Box)
	UStaticMeshComponent* Box;

	// 이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;

	// SHWeapon 클래스와 그걸 상속받는 클래스들의 목록으로 한정지음.
	UPROPERTY(EditInstanceOnly, Category = Box)
	TSubclassOf<class ASHWeapon> WeaponItemClass; // 전방선언

private:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);

};
