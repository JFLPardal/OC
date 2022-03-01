// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "DeliveryConveyorActor.h"
#include "RecipeData.h"

#include "RequestsSubsystem.generated.h"

class APlate;
class UDataTable;
struct FRecipes;

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGeneratedNewRequest, FRecipeData, GeneratedRequestData);

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCompletedRequest, FRecipeData, CompletedRequestData);

/**
 * Subsystem responsible for storage and the logic related with Requests
 */
UCLASS()
class OC_API URequestsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	URequestsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FGeneratedNewRequest OnGeneratedNewRequest;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FCompletedRequest OnCompletedRequest;

	UFUNCTION()
	void CheckIfPlateHasActiveRecipe(APlate* Plate);
private:
	FRecipeData* GetRandomRecipeFromRecipeBook();
	void GenerateRecipe();

	UPROPERTY(VisibleAnywhere)
	UDataTable* RecipesDataTable;

	UPROPERTY(EditAnywhere)
	int maxNumberOfSimultaneousActiveRecipes = 3;

	TArray<FRecipes*> RecipeBook;
	TArray<FRecipeData*> ActiveRecipesData;
	FString RecipesDataTableAssetLocation;

	TArray<AActor*> DeliveryConveyorActors;

	FTimerHandle GenerateRecipeTimer;
};
