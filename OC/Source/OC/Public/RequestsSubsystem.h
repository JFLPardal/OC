// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "DeliveryConveyorActor.h"

#include "RequestsSubsystem.generated.h"

class APlate;
class UDataTable;
struct FRecipes;
struct FRecipeData;

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
	void CheckIfPlateHasActiveRecipe(APlate* Plate);
private:
	FRecipeData* GetRandomRecipeFromRecipeBook();

	UPROPERTY(VisibleAnywhere)
	UDataTable* RecipesDataTable;

	UPROPERTY(EditAnywhere)
	int maxNumberOfSimultaneousActiveRecipes = 3;

	TArray<FRecipes*> RecipeBook;
	TArray<FRecipeData*> ActiveRecipesData;
	FString RecipesDataTableAssetLocation;

	TArray<AActor*> DeliveryConveyorActors;
};
