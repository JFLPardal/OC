// Fill out your copyright notice in the Description page of Project Settings.


#include "EIngredient.h"

bool IngredientHelpers::IsValid(EIngredient Ingredient)
{
	return Ingredient != EIngredient::Invalid;
}

FString IngredientHelpers::ToString(EIngredient Ingredient)
{
	return UEnum::GetDisplayValueAsText(Ingredient).ToString();
}
