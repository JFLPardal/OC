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
			WidgetTransform.Translation = FVector2D(550.0f, 150.0f);
			ActiveRecipeWidget->SetRenderTransform(WidgetTransform);
			ActiveRecipeWidget->SetColorAndOpacity(FLinearColor::Green);

			ActiveRecipeWidget->AddToViewport();
		}
	}
}

void AOCGameModeBase::CompletedRequest(FRecipeData CompletedRequestData)
{
	ActiveRecipeWidget->SetVisibility(ESlateVisibility::Hidden);
}
