// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveRecipeWidget.h"

#include "UIIngredientSlotWidget.h"

void UActiveRecipeWidget::SetRecipeData(FRecipeData Recipe)
{
	RecipeData = Recipe;
	for (auto Ingredient : Recipe.GetIngredients())
	{
		check(IngredientSlotWidgetBP);

		IngredientSlotWidget = Cast<UUIIngredientSlotWidget>(CreateWidget(GetWorld(), IngredientSlotWidgetBP));
		if (IngredientSlotWidget)
		{
			ActiveRecipeWidgetDataSource dataSource{ Ingredient };		
			IngredientSlotWidget->SetDataSource(dataSource);
			
			AddWidgetToHUD();
		}
	}
}