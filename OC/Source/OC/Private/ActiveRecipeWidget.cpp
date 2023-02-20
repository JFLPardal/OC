// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ActiveRecipeWidget.h"
#include "Animation\WidgetAnimation.h"
#include "Components\VerticalBox.h"
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
            
            if (IngredientsList)
            {
                IngredientsList->AddChild(IngredientSlotWidget);
            }
        }
    }
    
    PlayAnimation(Show);

    OnAddedRequestWidgetToHUD.Broadcast(RecipeData);
}

void UActiveRecipeWidget::Completed()
{
    PlayAnimation(Hide);
}

void UActiveRecipeWidget::OnAnimationFinished_Implementation(UWidgetAnimation const* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == Hide)
    {
        FinishedPlayingHideAnimation();
    }
}

void UActiveRecipeWidget::FinishedPlayingHideAnimation()
{
    OnFinishedHidingRequestOnHUD.Broadcast(RecipeData);
}

void UActiveRecipeWidget::PlayAnimation(UWidgetAnimation* AnimationToPlay)
{
    float const StartAtTime = 0.0f;
    int32 const NumOfLoopsToPlay = 1;
    EUMGSequencePlayMode::Type const SequencePlayMode = EUMGSequencePlayMode::Type::Forward;
    float const PlaybackSpeed = AnimationsPlaybackSpeed;
    bool const bRestoreState = true;

    UUserWidget::PlayAnimation(AnimationToPlay, StartAtTime, NumOfLoopsToPlay, SequencePlayMode, PlaybackSpeed, bRestoreState);
}