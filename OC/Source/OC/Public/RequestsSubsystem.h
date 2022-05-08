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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGeneratedNewRequest, const FRecipeData&, GeneratedRequestData);

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

	UFUNCTION(BlueprintCallable)
	TArray<EIngredient> GetIngredientsList(const FRecipeData& recipeData) const;
private:
	FRecipeData* GetRandomRecipeFromRecipeBook();
	TSharedPtr<FRecipeData> GetSharedPtrToRandomRecipeFromRecipeBook();

	void GenerateRecipe();

	UPROPERTY(VisibleAnywhere)
	UDataTable* RecipesDataTable;

	UPROPERTY(EditAnywhere)
	int maxNumberOfSimultaneousActiveRecipes = 3;

	FRecipeData dsd;
	TArray<FRecipes*> RecipeBook;
	TArray<FRecipeData*> ActiveRecipesData;
	//TArray<TSharedPtr<FRecipeData>> ActiveRecipesData;
	FString RecipesDataTableAssetLocation;

	TArray<AActor*> DeliveryConveyorActors;

	FTimerHandle GenerateRecipeTimer;
};
