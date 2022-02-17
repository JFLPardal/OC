// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "DeliveryConveyorActor.h"
// remove after CheckIfPlateHasActiveRecipe is updated to use Recipes
#include "EIngredient.h"

#include "RequestsSubsystem.generated.h"

class UDataTable;
struct FRecipes;
/**
 * Subsystem responsible for storing and all the logic related with Requests
 */
UCLASS()
class OC_API URequestsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	URequestsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void CheckIfPlateHasActiveRecipe(EIngredient Recipe);
private:
	FRecipes* GetRandomRecipeFromRecipeBook();

	UPROPERTY(VIsibleAnywhere)
	UDataTable* RecipesDataTable;

	TArray<FRecipes*> RecipeBook;
	FRecipes* CurrentRecipeData;
	FString RecipesDataTableAssetLocation;

	TArray<AActor*> DeliveryConveyorActors;
};
