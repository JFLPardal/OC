// Copyright Epic Games, Inc. All Rights Reserved.


#include "OCGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Macros.h"
#include "RequestsSubsystem.h"

void AOCGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (auto World = GetWorld())
	{
		if (auto GameInstance = World->GetGameInstance())
		{
			auto RequestsSubsystem = GameInstance->GetSubsystem<URequestsSubsystem>();

			RequestsSubsystem->OnGeneratedNewRequest.AddDynamic(this, &AOCGameModeBase::GeneratedNewRequest);
			RequestsSubsystem->OnCompletedRequest.AddDynamic(this, &AOCGameModeBase::CompletedRequest);
		}
	}
}

void AOCGameModeBase::GeneratedNewRequest(const FRecipeData& GeneratedRequestData)
{
	if (ensureMsgf(RecipeWidget, TEXT("RecipeWidget not set in GameMode")))
	{
		ActiveRecipeWidget = CreateWidget(GetWorld(), RecipeWidget);
		if (ActiveRecipeWidget)
		{
			// TODO set ActiveRecipeWidget's RecipeData to GeneratedRequestData
			// this needs ActiveRecipe to be a C++ class as well
			ActiveRecipeWidget->AddToViewport();
		}
	}
}

void AOCGameModeBase::CompletedRequest(FRecipeData CompletedRequestData)
{
	DTOS("request complete");
}
