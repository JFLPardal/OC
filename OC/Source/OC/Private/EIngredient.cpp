// Fill out your copyright notice in the Description page of Project Settings.


#include "EIngredient.h"

namespace IngredientHelpers
{
TMap<EIngredient, FText> IngredientToRepresentation
{
	{ EIngredient::Tomato, FText::FromString("T") },
	{ EIngredient::Lettuce, FText::FromString("L") },
	{ EIngredient::Onion, FText::FromString("O") },
	{ EIngredient::Beans, FText::FromString("B") },
	{ EIngredient::Invalid, FText::FromString("-") },
};

bool IsValid(EIngredient Ingredient)
{
	return Ingredient != EIngredient::Invalid;
}

FString ToString(EIngredient Ingredient)
{
	return UEnum::GetDisplayValueAsText(Ingredient).ToString();
}

FText* GetRepresentation(EIngredient Ingredient)
{
	FText* IngredientRepresentation = IngredientToRepresentation.Find(Ingredient);
	if (!ensureMsgf(IngredientRepresentation, TEXT("No entry found in IngredientToRepresentation for ingredient - %s.\nDid you forget to add it?"), *ToString(Ingredient)))
	{
		IngredientRepresentation = IngredientToRepresentation.Find(EIngredient::Invalid);
	}

	return IngredientRepresentation;
}
}
