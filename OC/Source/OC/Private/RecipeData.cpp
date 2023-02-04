// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeData.h"

FRecipeData::FRecipeData()
{
}

FRecipeData::FRecipeData(const FRecipeData& other)
{
    RecipeIngredients = other.RecipeIngredients;
}

bool FRecipeData::operator==(FRecipeData const& Recipe)
{
    return this->GetIngredients() == Recipe.GetIngredients();
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
    /*for (int i = 0; i < RecipeIngredients.Num(); ++i)
    {
        const bool isIngredientInRecipe = RecipeIngredients[i];
        if (isIngredientInRecipe)
        {
            IngredientList.Add(static_cast<EIngredient>(i));
        }
    }*/

    return IngredientList;
}


FRecipeData::~FRecipeData()
{
}
