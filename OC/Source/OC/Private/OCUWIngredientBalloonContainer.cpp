// Fill out your copyright notice in the Description page of Project Settings.


#include "OCUWIngredientBalloonContainer.h"

#include "Components\Image.h"

bool UOCUWIngredientBalloonContainer::CanAssignIngredient()
{
    bool const canAssign = !IngredientHelpers::IsValid(AssignedIngredient);
    return canAssign;
}

void UOCUWIngredientBalloonContainer::AssignIngredient(EIngredient Ingredient)
{
    ensureMsgf(IngredientHelpers::IsValid(Ingredient), TEXT("Trying to assign 'EIngredient::Invalid' to Ingredient in UOCUWIngredientBalloonContainer"));
    AssignedIngredient = Ingredient;

    FString SimpleRecipesDataTableAssetLocation{ "/Script/Engine.Texture2D'/Game/Interactables/Ingredients/T_Ing_Carrot.T_Ing_Carrot'" };
    auto IngredientTexture = LoadObject<UTexture2D>(NULL, *SimpleRecipesDataTableAssetLocation);
    if (ensureMsgf(IngredientTexture, TEXT("couldn't load Ingredient texture on location %s"), *SimpleRecipesDataTableAssetLocation))
    {
        Representation->SetBrushFromTexture(IngredientTexture);
        
    }
    SetVisibility(ESlateVisibility::Visible);
}

void UOCUWIngredientBalloonContainer::ClearAssignedIngredient()
{
    AssignedIngredient = EIngredient::Invalid;
    SetVisibility(ESlateVisibility::Hidden);
}
