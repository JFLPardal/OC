// Copyright Epic Games, Inc. All Rights Reserved.


#include "OCGameModeBase.h"

#include "ActiveRecipeWidget.h"
#include "Macros.h"
#include "RequestsSubsystem.h"

PRAGMA_DISABLE_OPTIMIZATION

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

		ActiveRequestsHUDElement = CreateWidget(GetWorld(), ActiveRequestsHUDElementBlueprint);
	}

	UE_VLOG(this, TEXT("GameModeCategory"), Verbose, TEXT("begin play"));
}

bool AOCGameModeBase::AreRecipesTheSame(FRecipeData const& Recipe, FRecipeData const& OtherRecipe) const
{
	return Recipe == OtherRecipe;
}

void AOCGameModeBase::GeneratedNewRequest(const FRecipeData& GeneratedRequestData)
{
	if (ensureMsgf(ActiveRecipeWidgetBlueprint, TEXT("RecipeWidget not set in GameMode")))
	{
		UActiveRecipeWidget* ActiveRecipeWidget { Cast<UActiveRecipeWidget>(CreateWidget(GetWorld(), ActiveRecipeWidgetBlueprint)) };

		if (ActiveRecipeWidget)
		{
			ActiveRecipeWidget->SetRecipeData(GeneratedRequestData);
			ActiveRecipeWidget->AddToViewport();

			int32 index = ActiveRequestWidgetsArray.Add(ActiveRecipeWidget);
			OnCreatedRequestWidget.Broadcast(ActiveRequestWidgetsArray[index]);

			TArray<EIngredient> const IngredientsOfNewRecipe = ActiveRecipeWidget->GetRecipeData().GetIngredients();

			UE_VLOG(this, TEXT("Requests"), Verbose, TEXT("Generated New RecipeWidget with [%s %s %s], %d RecipeWidgets generated"), 
				*UEnum::GetDisplayValueAsText(IngredientsOfNewRecipe[0]).ToString(),
				IngredientsOfNewRecipe.IsValidIndex(1) ? *UEnum::GetDisplayValueAsText(IngredientsOfNewRecipe[1]).ToString() : TEXT(""),
				IngredientsOfNewRecipe.IsValidIndex(2) ? *UEnum::GetDisplayValueAsText(IngredientsOfNewRecipe[2]).ToString() : TEXT(""),
				ActiveRequestWidgetsArray.Num());
		}
	}
}

void AOCGameModeBase::CompletedRequest(FRecipeData CompletedRequestData)
{	
	int32 CompletedRequestIndex = ActiveRequestWidgetsArray.IndexOfByPredicate([&CompletedRequestData](UActiveRecipeWidget const *const RecipeWidget) {return RecipeWidget->GetRecipeData() == CompletedRequestData; });
	
	if (ActiveRequestWidgetsArray.IsValidIndex(CompletedRequestIndex))
	{
		ActiveRequestWidgetsArray[CompletedRequestIndex]->Completed();
		ActiveRequestWidgetsArray.RemoveAt(CompletedRequestIndex);
		
		{ // debug
			TArray<EIngredient> const CompletedIngredients = CompletedRequestData.GetIngredients();

			UE_VLOG(this, TEXT("Requests"), Verbose, TEXT("Completed request with [%s %s %s], %d active requests remaining"),
					*UEnum::GetDisplayValueAsText(CompletedIngredients[0]).ToString(),
					CompletedIngredients.IsValidIndex(1) ? *UEnum::GetDisplayValueAsText(CompletedIngredients[1]).ToString() : TEXT(""),
					CompletedIngredients.IsValidIndex(2) ? *UEnum::GetDisplayValueAsText(CompletedIngredients[2]).ToString() : TEXT(""),
					ActiveRequestWidgetsArray.Num());
		}
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

	PlaceableCategory.Add(TEXT("NumberOfGeneratedRecipes"), FString::FromInt(ActiveRequestWidgetsArray.Num()));
}
#endif