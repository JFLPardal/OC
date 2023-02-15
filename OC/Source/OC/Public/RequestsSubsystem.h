// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "DeliveryConveyorActor.h"
#include "RecipeData.h"

#include "RequestsSubsystem.generated.h"

class APlate;
struct FRecipes;
class UDataTable;
class URequestSubsystemPOD;

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

	void DebugGenerateNewRequest();
	void DebugCompleteOldestRequest();
private:
	UPROPERTY(VisibleAnywhere)
	UDataTable* RecipesDataTable;

	uint8 maxNumberOfSimultaneousActiveRecipes = 3;

	TArray<FRecipes*> RecipeBook;
	TArray<TSharedPtr<FRecipeData>> ActiveRecipes;
	FString RecipesDataTableAssetLocation;

	TArray<AActor*> DeliveryConveyorActors;


	URequestSubsystemPOD* InitData;

	FTimerHandle GenerateRecipeTimer;
	FTimerManager* TimerManager;
private:
	void SetInitData();
	void CreateRecipesDataTable();

	TSharedPtr<FRecipeData> GetSharedPtrToRandomRecipeFromRecipeBook();
	
	void GenerateRecipe();

	// TODO create an interface with these 4 functions so we can have time based generation and some other type we might want in the future
	void StartRequestGeneration(bool bIsFirstTimeGenerating);
	void PauseRequestGeneration();
	bool IsRequestGenerationPaused();
	void ResumeRequestGeneration();
};
