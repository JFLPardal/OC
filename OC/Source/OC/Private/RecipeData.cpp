// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeData.h"

FRecipeData::FRecipeData()
{
}

bool FRecipeData::CanAddIngrendient(EIngredient IngredientToAdd) const
{
    bool RecipeAlreadyHasIngredient = false;
    const auto IngredientPositionInArray = (uint8)IngredientToAdd;

    if(ensureMsgf(IngredientPositionInArray < RecipeIngredients.Num(), TEXT("There can only be 16 different ingredients, ingredient provided exceeds this value")))
    {
        RecipeAlreadyHasIngredient = RecipeIngredients[IngredientPositionInArray] == true;
    }
    return !RecipeAlreadyHasIngredient;
}

void FRecipeData::AddIngredient(EIngredient IngredientToAdd)
{
    FString Ingredients = RecipeIngredients.ToString();
    UE_LOG(LogTemp, Warning, TEXT("before adding ingredient %s"), *Ingredients);

    if(CanAddIngrendient(IngredientToAdd))
    {
        const auto IngredientPositionInArray = (uint8)IngredientToAdd;
        RecipeIngredients[IngredientPositionInArray] = true;
    }
    
    Ingredients = RecipeIngredients.ToString();
    UE_LOG(LogTemp, Warning, TEXT("after adding ingredient %s"), *Ingredients);
}

FRecipeData::~FRecipeData()
{
}
