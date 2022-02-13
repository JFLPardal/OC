// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// debug to screen & screen with color
#define DTOS(text) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT(text)); }
#define DTOSC(text, color) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 3.0f, color, TEXT(text)); }

// log 
#define LOG_WARNING(text) UE_LOG(LogTemp, Warn, TEXT(text))

// ensure 
#define ENSURE_MSG(condition, text) ensureMsgf(condition, TEXT(text))