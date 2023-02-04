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
		ActiveRecipeWidget = Cast<UActiveRecipeWidget>(CreateWidget(GetWorld(), ActiveRecipeWidgetBlueprint));
		if (ActiveRecipeWidget)
		{
			ActiveRecipeWidget->SetRecipeData(GeneratedRequestData);
			
			// Widget's position
			FWidgetTransform WidgetTransform;
			WidgetTransform.Translation = FVector2D(125.0f * NumberOfGeneratedRecipeWidgets,0.0f );
			ActiveRecipeWidget->SetRenderTransform(WidgetTransform);
			ActiveRecipeWidget->SetColorAndOpacity(FLinearColor::Gray);

			ActiveRecipeWidget->AddToViewport();

			++NumberOfGeneratedRecipeWidgets;
			UE_VLOG(this, TEXT("GameModeCategory"), Verbose, TEXT("Generated New RecipeWidget, %d RecipeWidgets generated"), NumberOfGeneratedRecipeWidgets);
		}
	}
}

void AOCGameModeBase::CompletedRequest(FRecipeData CompletedRequestData)
{
	ActiveRecipeWidget->SetVisibility(ESlateVisibility::Hidden);
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

	PlaceableCategory.Add(TEXT("NumberOfGeneratedRecipes"), FString::FromInt(NumberOfGeneratedRecipeWidgets));
}
#endif