// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeData.h"

FRecipeData::FRecipeData()
{
}

FRecipeData::FRecipeData(const FRecipeData& other)
{
    RecipeIngredients = other.RecipeIngredients;
}

bool FRecipeData::AreRecipesTheSame(FRecipeData const& Recipe, FRecipeData const& OtherRecipe)
{
    return Recipe == OtherRecipe;
}

bool operator==(FRecipeData const& Recipe, FRecipeData const& AnotherRecipe)
{
    return Recipe.GetIngredients() == AnotherRecipe.GetIngredients();
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
    if(CanAddIngrendient(IngredientToAdd))
    {
        const auto IngredientPositionInArray = (uint8)IngredientToAdd;
        RecipeIngredients[IngredientPositionInArray] = true;
    }
}

TArray<EIngredient> FRecipeData::GetIngredients() const
{
    TArray<EIngredient> IngredientList{};
    for (int32 Index = RecipeIngredients.NumOfBits - 1; Index >= 0; --Index)
    {
        if (RecipeIngredients[Index])
        {
            IngredientList.Add(static_cast<EIngredient>(Index));
        }
    }

    return IngredientList;
}


FRecipeData::~FRecipeData()
{
}
