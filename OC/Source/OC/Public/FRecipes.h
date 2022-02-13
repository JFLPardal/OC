// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "EIngredient.h"
#include "CoreMinimal.h"

#include "FRecipes.generated.h"

/**
 * Defines the necessary data for each Recipe
 */
USTRUCT(BlueprintType)
struct FRecipes : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIngredient Ingredient_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIngredient Ingredient_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIngredient Ingredient_3;
};
