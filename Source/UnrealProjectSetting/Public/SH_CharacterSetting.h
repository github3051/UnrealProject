// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SH_CharacterSetting.generated.h"

/**
 * 
 */
// 불러들일 UnrealProject의 이름인 INI 파일명을 지정.
UCLASS(config=UnrealProject)
class UNREALPROJECTSETTING_API USH_CharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	USH_CharacterSetting();

	// 캐릭터 애셋 정보
	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
};
