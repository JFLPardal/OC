// Fill out your copyright notice in the Description page of Project Settings.


#include "OCUWIngredientBalloonContainer.h"

bool UOCUWIngredientBalloonContainer::CanAssignIngredient()
{
    bool const canAssign = !IngredientHelpers::IsValid(AssignedIngredient);
    return canAssign;
}

void UOCUWIngredientBalloonContainer::AssignIngredient(EIngredient Ingredient)
{
    ensureMsgf(IngredientHelpers::IsValid(Ingredient), TEXT("Trying to assign 'EIngredient::Invalid' to Ingredient in UOCUWIngredientBalloonContainer"));
    AssignedIngredient = Ingredient;
}
