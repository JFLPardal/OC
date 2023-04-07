// Fill out your copyright notice in the Description page of Project Settings.


#include "OCUWIngredientBalloonContainer.h"

#include "Components\TextBlock.h"

bool UOCUWIngredientBalloonContainer::CanAssignIngredient()
{
    bool const canAssign = !IngredientHelpers::IsValid(AssignedIngredient);
    return canAssign;
}

void UOCUWIngredientBalloonContainer::AssignIngredient(EIngredient Ingredient)
{
    ensureMsgf(IngredientHelpers::IsValid(Ingredient), TEXT("Trying to assign 'EIngredient::Invalid' to Ingredient in UOCUWIngredientBalloonContainer"));
    AssignedIngredient = Ingredient;
    Representation->SetText(*IngredientHelpers::GetRepresentation(AssignedIngredient));

    SetVisibility(ESlateVisibility::Visible);
}

void UOCUWIngredientBalloonContainer::ClearAssignedIngredient()
{
    AssignedIngredient = EIngredient::Invalid;
    SetVisibility(ESlateVisibility::Hidden);
}
