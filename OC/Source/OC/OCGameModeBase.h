// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "RecipeData.h"

#include "OCGameModeBase.generated.h"

/**
 * 
 */

class UUserWidget;

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
	UPROPERTY(EditAnywhere, Category="RecipeUI")
	TSubclassOf<UUserWidget> RecipeWidget;

	UUserWidget* ActiveRecipeWidget;
};
