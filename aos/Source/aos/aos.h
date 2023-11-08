// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//ȣ��� �Լ��� �̸��� ���� ��ũ�θ� String ȭ



#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(" + FString::FromInt(__LINE__) + TEXT(")")))

//#define LOG_S(CategoryName,Verbosity) UE_LOG(CategoryName,Verbosity,TEXT("%s"),*LOG_CALLINFO)

#define LOG(Category, Verbosity, Format, ...) UE_LOG(Category, Verbosity, TEXT("%s %s: %s"), *LOG_CALLINFO, TEXT(#Category), *FString::Printf(Format, ##__VA_ARGS__))

//#define LOG(Category,Verbosity,Format,...) UE_LOG(Category, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define LOG(Verbosity, Format, ...) UE_LOG(LogTemp, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

