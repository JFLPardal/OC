// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Ingredients available
 */
UENUM(BlueprintType)
enum class EIngredient : uint8
{
	Aubergine 		UMETA(DisplayName="Aubergine"),
	Lettuce 		UMETA(DisplayName="Lettuce"),
	Pepper 			UMETA(DisplayName="Pepper"),
	Carrot 			UMETA(DisplayName="Carrot"),
	Brocolli		UMETA(DisplayName="Brocolli"),
	Potato			UMETA(DisplayName="Potato"),

	Invalid			UMETA(DisplayName = "INVALID"),
	Default = Lettuce
};

class UTexture2D;

namespace IngredientHelpers
{
	bool IsValid(EIngredient Ingredient);
	FString ToString(EIngredient Ingredient);
	FText* GetRepresentation(EIngredient Ingredient);
	UTexture2D* GetImageRepresentation(EIngredient Ingredient);
};
