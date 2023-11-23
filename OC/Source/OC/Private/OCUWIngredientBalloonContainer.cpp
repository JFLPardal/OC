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

    UTexture2D* const IngredientUITexture = IngredientHelpers::GetImageRepresentation(Ingredient);
    if (ensureMsgf(IngredientUITexture, TEXT("couldn't load Ingredient texture for Ingredient %s. Make sure the location referenced in EIngredient.cpp is valid"), *IngredientHelpers::ToString(Ingredient)))
    {
        Representation->SetBrushFromTexture(IngredientUITexture);
    }
    SetVisibility(ESlateVisibility::Visible);
}

void UOCUWIngredientBalloonContainer::ClearAssignedIngredient()
{
    AssignedIngredient = EIngredient::Invalid;
    SetVisibility(ESlateVisibility::Hidden);
}
