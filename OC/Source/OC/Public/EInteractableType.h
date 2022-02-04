// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Types of interactables in the game
 */
UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	Plate 						UMETA(DisplayName="Plate"),
	DeliveryConveyor 			UMETA(DisplayName="DeliveryConveyor"),
	Ingredient					UMETA(DisplayName="Ingredient"),
	IngredientSpawnerCrate 		UMETA(DisplayName="IngredientSpawnerCrate"),
	Unspecified 				UMETA(DisplayName="Unspecified"),
	Invalid 					UMETA(DisplayName="INVALID"),

};
