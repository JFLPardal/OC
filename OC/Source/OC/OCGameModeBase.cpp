// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "OCGameModeBase.h"

#include "ActiveRecipeWidget.h"
#include "RequestsSubsystem.h"
#include "UOCUWLevelTimerHUD.h"

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
		
		LevelTimerWidget = Cast<UUOCUWLevelTimerHUD>(CreateWidget(GetWorld(), LevelTimerWidgetBlueprint));
		if (ensureMsgf(LevelTimerWidget, TEXT("Couldn't create LevelTimerWidget - make sure a UUOCUWLevelTimerHUD blueprint is assigned in the level's bp")))
		{
			LevelTimerWidget->SetGameMode(this);
			LevelTimerWidget->AddToViewport();
		}

		SetLevelTimer();
	}

	UE_VLOG(this, TEXT("GameModeCategory"), Verbose, TEXT("begin play"));
}

void AOCGameModeBase::SetLevelTimer()
{
	TimerManager = &(GetWorld()->GetTimerManager());
	if (ensureMsgf(TimerManager, TEXT("TimerManager in AOCGameModeBase not set")))
	{
		float const rate = 1.0f;
		bool const shouldRepeat = true;

		TimerManager->SetTimer(
			TimerToFinishLevel,
			this,
			&AOCGameModeBase::DecreaseTimerRemainingInLevel,
			rate,
			shouldRepeat
		);
	}
	
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

void AOCGameModeBase::DecreaseTimerRemainingInLevel()
{
	--TimeRemainingInLevelInSecs;
	OnUpdatedTimeRemainingInLevel.Broadcast(TimeRemainingInLevelInSecs);
	bool const IsTimeUp = TimeRemainingInLevelInSecs <= 0.f && TimerManager->TimerExists(TimerToFinishLevel);
	if (IsTimeUp)
	{
		if (TimerManager)
		{
			TimerManager->ClearTimer(TimerToFinishLevel);
		}
		if (bShouldTimerExpiredFinishGame)
		{
			FinishLevel();
		}
	}
}

void AOCGameModeBase::FinishLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("Knives Down!"));
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
