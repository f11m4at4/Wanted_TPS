// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TPS, Log, All);

#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PRINT_CALLINFO() \
UE_LOG(TPS, Warning, TEXT("%s"), *CALLINFO)

#define PRINTLOG(fmt, ...) \
UE_LOG(TPS, Warning, TEXT("%s %s"), *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__))

#define PRINTLOGTOSCREEN(fmt, ...) \
PRINTLOG(fmt, ##__VA_ARGS__);\
GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, *FString::Printf(fmt, ##__VA_ARGS__))

inline void PrintLogFunc(const TCHAR* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TCHAR buffer[1024];
	FCString::GetVarArgs(buffer, UE_ARRAY_COUNT(buffer), fmt, args);
	PRINTLOG(TEXT("%s"), buffer);

	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, *FString::Printf(TEXT("%s"), buffer));

	va_end(args);
}
