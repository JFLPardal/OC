// Fill out your copyright notice in the Description page of Project Settings.


#include "OCWBalloon.h"
#include "Components\CanvasPanel.h"

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
