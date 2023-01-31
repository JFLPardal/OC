// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "RecipeData.h"

#include "OCGameModeBase.generated.h"

/**
 * 
 */

class UActiveRecipeWidget;
class URequestsSubsystem;

UCLASS()
class OC_API AOCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void GeneratedNewRequest(const FRecipeData& GeneratedRequestData);
	UFUNCTION()
	void CompletedRequest(FRecipeData CompletedRequestData);
private:
	URequestsSubsystem* RequestsSubsystem;
	
	UPROPERTY(EditAnywhere, Category = "RecipeUI")
	TSubclassOf<UActiveRecipeWidget> ActiveRecipeWidgetBlueprint;
	
	UActiveRecipeWidget* ActiveRecipeWidget;

	// TODO delete this when using a vector of ActiveRecipeWidget
	int NumberOfGeneratedRecipeWidgets = 0;
};
