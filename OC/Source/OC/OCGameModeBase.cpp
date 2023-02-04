// Copyright Epic Games, Inc. All Rights Reserved.


#include "OCGameModeBase.h"

#include "ActiveRecipeWidget.h"
#include "Macros.h"
#include "RequestsSubsystem.h"

void AOCGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (auto World = GetWorld())
	{
		if (auto GameInstance = World->GetGameInstance())
		{
			RequestsSubsystem = GameInstance->GetSubsystem<URequestsSubsystem>();
			if (RequestsSubsystem)
			{
				RequestsSubsystem->OnGeneratedNewRequest.AddDynamic(this, &AOCGameModeBase::GeneratedNewRequest);
				RequestsSubsystem->OnCompletedRequest.AddDynamic(this, &AOCGameModeBase::CompletedRequest);
			}
		}
	}

	UE_VLOG(this, TEXT("GameModeCategory"), Verbose, TEXT("begin play"));
}

void AOCGameModeBase::GeneratedNewRequest(const FRecipeData& GeneratedRequestData)
{
	if (ensureMsgf(ActiveRecipeWidgetBlueprint, TEXT("RecipeWidget not set in GameMode")))
	{
		UActiveRecipeWidget* ActiveRecipeWidget { Cast<UActiveRecipeWidget>(CreateWidget(GetWorld(), ActiveRecipeWidgetBlueprint)) };

		if (ActiveRecipeWidget)
		{
			ActiveRecipeWidget->SetRecipeData(GeneratedRequestData);
			
			// Widget's position
			FWidgetTransform WidgetTransform;
			
			WidgetTransform.Translation = FVector2D(125.0f * ActiveRecipeWidgetArray.Num(), 0.0f);
			
			ActiveRecipeWidget->SetRenderTransform(WidgetTransform);
			ActiveRecipeWidget->SetColorAndOpacity(FLinearColor::Gray);
			ActiveRecipeWidget->AddToViewport();

			ActiveRecipeWidgetArray.Add(ActiveRecipeWidget);

			TArray<EIngredient> IngredientsOfNewRecipe = ActiveRecipeWidget->GetRecipeData().GetIngredients();

			UE_VLOG(this, TEXT("GameModeCategory"), Verbose, TEXT("Generated New RecipeWidget with [%s %s %s], %d RecipeWidgets generated"), 
				*UEnum::GetDisplayValueAsText(IngredientsOfNewRecipe[0]).ToString(),
				IngredientsOfNewRecipe.IsValidIndex(1) ? *UEnum::GetDisplayValueAsText(IngredientsOfNewRecipe[1]).ToString() : TEXT(""),
				IngredientsOfNewRecipe.IsValidIndex(2) ? *UEnum::GetDisplayValueAsText(IngredientsOfNewRecipe[2]).ToString() : TEXT(""),
				ActiveRecipeWidgetArray.Num());
		}
	}
}

void AOCGameModeBase::CompletedRequest(FRecipeData CompletedRequestData)
{
	auto CompletedRecipeIt = ActiveRecipeWidgetArray.begin();
	for (; CompletedRecipeIt != ActiveRecipeWidgetArray.end(); ++CompletedRecipeIt)
	{
		if ((*CompletedRecipeIt)->GetRecipeData() == CompletedRequestData)
		{
			(*CompletedRecipeIt)->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (CompletedRecipeIt != ActiveRecipeWidgetArray.end())
	{
		ActiveRecipeWidgetArray.Remove(*CompletedRecipeIt);
	}
}

void AOCGameModeBase::DebugCompleteOldestRequest()
{
	if (RequestsSubsystem)
	{
		RequestsSubsystem->DebugCompleteOldestRequest();
	}
}

void AOCGameModeBase::DebugGenerateNewRequest()
{
	if (RequestsSubsystem) 
	{
		RequestsSubsystem->DebugGenerateNewRequest();
	}
}

#if ENABLE_VISUAL_LOG
void AOCGameModeBase::GrabDebugSnapshot(FVisualLogEntry* Snapshot) const
{
	IVisualLoggerDebugSnapshotInterface::GrabDebugSnapshot(Snapshot);

	const int32 CategoryIndex = Snapshot->Status.AddZeroed();
	FVisualLogStatusCategory& PlaceableCategory = Snapshot->Status[CategoryIndex];
	PlaceableCategory.Category = TEXT("AOCGameModeBase");

	PlaceableCategory.Add(TEXT("NumberOfGeneratedRecipes"), FString::FromInt(ActiveRecipeWidgetArray.Num()));
}
#endif