// Fill out your copyright notice in the Description page of Project Settings.


#include "EIngredient.h"

#include "Engine/Texture2D.h"

namespace IngredientHelpers
{
TMap<EIngredient, FText> IngredientToRepresentation
{
	{ EIngredient::Aubergine, FText::FromString("A") },
	{ EIngredient::Lettuce, FText::FromString("L") },
	{ EIngredient::Pepper, FText::FromString("P") },
	{ EIngredient::Carrot, FText::FromString("C") },
	{ EIngredient::Brocolli, FText::FromString("B") },
	{ EIngredient::Potato, FText::FromString("T") },
	{ EIngredient::Invalid, FText::FromString("-") },
};

FString DefaultIngredientUITextureLocation{ "/Script/Engine.Texture2D'/Game/Interactables/Ingredients/" };
FString AubergineUITextureLocation{ DefaultIngredientUITextureLocation + "T_Ing_Aubergine.T_Ing_Aubergine'" };
FString LettuceUITextureLocation{ "/Script/Engine.Texture2D'/Game/Interactables/Ingredients/T_Ing_Lettuce.T_Ing_Lettuce'" };
FString PepperUITextureLocation{ "/Script/Engine.Texture2D'/Game/Interactables/Ingredients/T_Ing_Pepper.T_Ing_Pepper'" };
FString CarrotUITextureLocation{ "/Script/Engine.Texture2D'/Game/Interactables/Ingredients/T_Ing_Carrot.T_Ing_Carrot'" };
FString BrocolliUITextureLocation{ "/Script/Engine.Texture2D'/Game/Interactables/Ingredients/T_Ing_Brocolli.T_Ing_Brocolli'" };
FString PotatoUITextureLocation{ "/Script/Engine.Texture2D'/Game/Interactables/Ingredients/T_Ing_Potato.T_Ing_Potato'" };

#define INGREDIENT_TO_UI_TEXTURE_ENTRY(Ingredient) \
 {EIngredient::Ingredient, LoadObject<UTexture2D>(NULL, *Ingredient##UITextureLocation) }

TMap<EIngredient, UTexture2D*> IngredientToUITexture
{
	INGREDIENT_TO_UI_TEXTURE_ENTRY(Aubergine),
	INGREDIENT_TO_UI_TEXTURE_ENTRY(Lettuce),
	INGREDIENT_TO_UI_TEXTURE_ENTRY(Pepper),
	INGREDIENT_TO_UI_TEXTURE_ENTRY(Carrot),
	INGREDIENT_TO_UI_TEXTURE_ENTRY(Brocolli),
	INGREDIENT_TO_UI_TEXTURE_ENTRY(Potato)
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

UTexture2D* GetImageRepresentation(EIngredient Ingredient)
{
	UTexture2D** IngredientRepresentation = IngredientToUITexture.Find(Ingredient);
	if (!ensureMsgf(IngredientRepresentation, TEXT("No entry found in IngredientToUITexture for ingredient - %s.\nDid you forget to add it? Is the location referenced correct?"), *ToString(Ingredient)))
	{
		IngredientRepresentation = nullptr;
	}
	
	return *IngredientRepresentation;
}
}
