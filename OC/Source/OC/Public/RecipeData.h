// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/StaticBitArray.h"
#include "EIngredient.h"
#include "RecipeData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRecipeData
{
	GENERATED_USTRUCT_BODY()
public:
	FRecipeData();
	~FRecipeData();

	bool CanAddIngrendient(EIngredient IngredientToAdd) const;
	void AddIngredient(EIngredient IngredientToAdd);
//private:
	TStaticBitArray<16> RecipeIngredients;
};
