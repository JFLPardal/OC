// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "UOCUWActiveRequestsHUD.h"

#include "../OCGameModeBase.h"
#include "ActiveRecipeWidget.h"
#include "Components\HorizontalBox.h"
#include "Kismet/GameplayStatics.h"

bool UUOCUWActiveRequestsHUD::Initialize()
{
	Super::Initialize();
	
	AOCGameModeBase* GameMode = Cast<AOCGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (ensureMsgf(GameMode, TEXT("Failed to cast GameMode to AOCGameModeBase")))
	{
		GameMode->OnCreatedRequestWidget.AddDynamic(this, &UUOCUWActiveRequestsHUD::OnRequestWidgetCreated);
	}

	AddToViewport();

	return true;
}

void UUOCUWActiveRequestsHUD::OnRequestWidgetCreated(UActiveRecipeWidget* CreatedWidget)
{
	if (ensureMsgf(CreatedWidget, TEXT("OnRequestWidgetCreated called with an empty CreatedWidget")))
	{
		ActiveRequestWidgetsList->AddChild(CreatedWidget);

		CreatedWidget->OnFinishedHidingRequestOnHUD.AddDynamic(this, &UUOCUWActiveRequestsHUD::OnFinishedHidingRequest);
	}
}

void UUOCUWActiveRequestsHUD::OnFinishedHidingRequest(FRecipeData RemovedRecipeData)
{
	RemoveRequestFromActiveRequestWidgetList(RemovedRecipeData);
}

void UUOCUWActiveRequestsHUD::RemoveRequestFromActiveRequestWidgetList(FRecipeData& RemovedRecipeData)
{
	int32 const InvalidIndex = -1;
	int32 IndexToRemove = InvalidIndex;

	for (int32 i = 0; i < ActiveRequestWidgetsList->GetChildrenCount(); ++i)
	{
		UActiveRecipeWidget const* const ActiveRequestWidget = Cast<UActiveRecipeWidget>(ActiveRequestWidgetsList->GetChildAt(i));
		if (ensureMsgf(ActiveRequestWidget, TEXT("Couldn't cast Widget in ActiveRequestWidgetsList to UActiveRecipeWidget")))
		{
			FRecipeData const& ActiveRecipe = ActiveRequestWidget->GetRecipeData();
			bool const IsActiveRecipeTheRecipeToRemove = FRecipeData::AreRecipesTheSame(ActiveRecipe, RemovedRecipeData);

			if (IsActiveRecipeTheRecipeToRemove)
			{
				IndexToRemove = i;
				break;
			}
		}
	}

	bool const IsIndexValid =
		IndexToRemove != InvalidIndex
		&& IndexToRemove >= 0
		&& IndexToRemove < ActiveRequestWidgetsList->GetChildrenCount();
	if (IsIndexValid)
	{
		ActiveRequestWidgetsList->RemoveChildAt(IndexToRemove);
	}
}
