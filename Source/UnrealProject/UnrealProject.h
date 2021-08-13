// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "EngineMinimal.h" // 더 많은 기능을 위해 (0811)


#pragma region SungHoon area
// LOG macro by SungHoon
DECLARE_LOG_CATEGORY_EXTERN(LOG_SH, Log, All);

// 밑의 2개에 넣기 위한 매크로
#define SHLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(" line)"))

/// 실제로 사용하게될 매크로 명. UE_LOG 문법을 가져와야함.
// 어느 함수의 몇줄에서 실행됐는지만 출력
#define SH_LOG_S(Verbosity) UE_LOG(LOG_SH, Verbosity, TEXT("%s"), *SHLOG_CALLINFO)
// printf 문법을 추가한 기능. UE_LOG와 완전히 동일함. 
#define SH_LOG(Verbosity, Format, ...) UE_LOG(LOG_SH, Verbosity, TEXT("%s, %s"), *SHLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#pragma endregion
