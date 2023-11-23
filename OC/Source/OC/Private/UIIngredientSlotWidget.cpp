// Fill out your copyright notice in the Description page of Project Settings.


#include "UIIngredientSlotWidget.h"

void UUIIngredientSlotWidget::SetDataSource(ActiveRecipeWidgetDataSource DataSourceToSet)
{
	FText text = UEnum::GetDisplayValueAsText(DataSourceToSet.GetIngredient()); 
	IngredientName = text;
}

ActiveRecipeWidgetDataSource::ActiveRecipeWidgetDataSource()
	:ActiveRecipeWidgetDataSource(EIngredient::Default)
{
}

ActiveRecipeWidgetDataSource::ActiveRecipeWidgetDataSource(EIngredient IngredientToSet)
	:Ingredient(IngredientToSet)
{
}
