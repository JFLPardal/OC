// Fill out your copyright notice in the Description page of Project Settings.


#include "OCWBalloon.h"

#include "Components\CanvasPanel.h"
#include "Components\UniformGridPanel.h"
#include "IIngredientContainer.h"
#include "Ingredient.h"

bool UOCWBalloon::Initialize()
{
    Super::Initialize();
    
    //Hide();

    return true;
}


void UOCWBalloon::Show()
{
    SetCanvasVisibility(ESlateVisibility::Visible);
}

void UOCWBalloon::Hide()
{
    SetCanvasVisibility(ESlateVisibility::Collapsed);
}

void UOCWBalloon::SetCanvasVisibility(ESlateVisibility VisibilityToSet)
{
    if (CanvasPanel)
    {
        CanvasPanel->SetVisibility(VisibilityToSet);
    }
}

void UOCWBalloon::UpdateIngredientsWidget(AInteractableActor const* const NewPlateComposition)
{
	for (UWidget* const Child : Collection->GetAllChildren())
	{
		auto IIngredientContainer = Cast<IIIngredientContainer>(Child);
		if (ensureMsgf(IIngredientContainer, TEXT("Collection in UOCUBallonComponent has at least one child element that is not implementing IIIngredientContainer")))
		{
			if (IIngredientContainer->CanAssignIngredient())
			{
				AIngredient const* const IngredientActor = Cast<AIngredient>(NewPlateComposition);
				if (ensureMsgf(IngredientActor, TEXT("Could not convert %s to AIngredient"), *NewPlateComposition->GetName()))
				{
					EIngredient IngredientToAssign = IngredientActor->GetIngredient();

					IIngredientContainer->AssignIngredient(IngredientToAssign);
					UE_LOG(LogTemp, Error, TEXT("adding %s to the plate"), *IngredientHelpers::ToString(IngredientToAssign));
					return;
				}
				return;
			}
		}
	}
}