// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/StaticBitArray.h"
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
//private:
	TStaticBitArray<16> RecipeIngredients;
};
