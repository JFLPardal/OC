// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveRecipeWidget.h"

#include "UIIngredientSlotWidget.h"
#include "Components\CanvasPanel.h"
#include "Components\VerticalBox.h"

bool UActiveRecipeWidget::Initialize()
{
    Super::Initialize();

    IngredientsList = NewObject<UVerticalBox>(this, UVerticalBox::StaticClass());
    UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(GetRootWidget());

    if (CanvasPanel)
    {
        CanvasPanel->AddChild(IngredientsList);
    }
    return true;
}

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
                IngredientsList->AddChildToVerticalBox(IngredientSlotWidget);
            }
        }
    }
}