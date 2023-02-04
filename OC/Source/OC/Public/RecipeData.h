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
	FRecipeData(const FRecipeData&);

	~FRecipeData();

	bool CanAddIngrendient(EIngredient IngredientToAdd) const;
	void AddIngredient(EIngredient IngredientToAdd);

	TArray<EIngredient> GetIngredients() const;

	bool operator==(FRecipeData const& Recipe);
//private:
	TStaticBitArray<16> RecipeIngredients;
};
