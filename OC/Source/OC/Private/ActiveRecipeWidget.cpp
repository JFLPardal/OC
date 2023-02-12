// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ActiveRecipeWidget.h"

#include "UIIngredientSlotWidget.h"
#include "Components\VerticalBox.h"

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
            
            if (IngredientsList)
            {
                IngredientsList->AddChild(IngredientSlotWidget);
            }
        }
    }
}