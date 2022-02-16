// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestsSubsystem.h"

#include "Engine/DataTable.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/Class.h"

#include "Macros.h"
#include "FRecipes.h"

URequestsSubsystem::URequestsSubsystem()
{
    RecipesDataTableAssetLocation = "DataTable'/Game/DT_Recipes.DT_Recipes'";
    static ConstructorHelpers::FObjectFinder<UDataTable> RecipesDataTableAsset(*RecipesDataTableAssetLocation);
    
    if(ensureMsgf(RecipesDataTableAsset.Succeeded(), TEXT("RecipedDataTable %s not found - check the recipes' data table name"), *RecipesDataTableAssetLocation))
    {
        RecipesDataTable = RecipesDataTableAsset.Object;
        
        // Cache all recipes
        const FString contextString {TEXT("RequestsSubsystem - RecipesDataTable")}; 
        RecipesDataTable->GetAllRows<FRecipes>(contextString, RecipeBook);
    }
}

namespace OC
{
    enum class PrintTo
    {
        outputOnly,
        screenOnly,
        outputAndScreen,
    };

    void PrintRecipe(const FRecipes& Recipe, OC::PrintTo printLocation)
    {
        const auto GetStringValue = [](EIngredient IngredientAsEnum) -> FString {return UEnum::GetValueAsString(IngredientAsEnum); };
        const auto PrintToOutput = [&](const FRecipes& Recipe) 
        { 
            UE_LOG(LogTemp, Warning, TEXT(" \nFirst ingredient \t%s\nSecond ingredient \t%s\nThird ingredient \t%s\n"), 
                *GetStringValue(Recipe.FirstIngredient),
                *GetStringValue(Recipe.SecondIngredient),
                *GetStringValue(Recipe.ThirdIngredient)
            );
        };
        const auto PrintToScreen = [&](const FRecipes& Recipe) 
        {
            FString FullRecipe = GetStringValue(Recipe.FirstIngredient) + "\n" 
                            + GetStringValue(Recipe.SecondIngredient)+ "\n"
                            + GetStringValue(Recipe.ThirdIngredient);
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FullRecipe);
        };

        using namespace OC;
        switch(printLocation)
        {
            case PrintTo::outputOnly :
            {
                PrintToOutput(Recipe);
                break;
            }
            case PrintTo::screenOnly :
            {
                PrintToScreen(Recipe);
                break;
            }
            case PrintTo::outputAndScreen :
            {
                PrintToOutput(Recipe);
                PrintToScreen(Recipe);
                break;
            }
        }
    }
}

void URequestsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    if(ensureMsgf(RecipesDataTable, TEXT("URequestsSubsystem - RecipedDataTable is empty")))
    {
        CurrentRecipeData = GetRandomRecipeFromRecipeBook();
        OC::PrintRecipe(*CurrentRecipeData, OC::PrintTo::outputAndScreen);
    }
}

FRecipes* URequestsSubsystem::GetRandomRecipeFromRecipeBook()
{
    FRecipes* Recipe{nullptr};
    
    const auto numberOfRecipesInBook = RecipeBook.Num();
    if(ensureMsgf(numberOfRecipesInBook > 0, TEXT("Recipe Book has no recipes. Make sure %s has recipes"), *RecipesDataTableAssetLocation))
    {
        const int randomIndex = FMath::RandRange(0, RecipeBook.Num() - 1);
        Recipe = RecipeBook[randomIndex];
    }
    
    return Recipe;
}