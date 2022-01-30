// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Ingredients available
 */
UENUM(BlueprintType)
enum class EIngredient : uint8
{
	Tomato 		UMETA(DisplayName="Tomato"),
	Lettuce 	UMETA(DisplayName="Lettuce"),
	Invalid		UMETA(DisplayName="INVALID"),
};
