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
    if(ensureMsgf(RecipesDataTableAsset.Succeeded(), TEXT("RecipedDataTable not found in path ")))
    {
        RecipesDataTable = RecipesDataTableAsset.Object;
    }
}

void URequestsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    //setup RecipesDataTable
    if(ensureMsgf(RecipesDataTable, TEXT("URequestsSubsystem - RecipedDataTable is empty")))
    {
        const FString contextString {TEXT("RequestsSubsystem - RecipesDataTable")}; 
        CurrentRecipeData = RecipesDataTable->FindRow<FRecipes>(FName(TEXT("1")), contextString, true);
        if(CurrentRecipeData)
        {
            UE_LOG(LogTemp, Error, TEXT("First ingredient %d"), CurrentRecipeData->FirstIngredient);
        }

        {
            // Store all rows
            TArray<FRecipes*> RecipeBook;
            RecipesDataTable->GetAllRows<FRecipes>(contextString, RecipeBook);

            // get a random recipe from the book
            const auto numberOfRecipesInBook = RecipeBook.Num();
            if(ensureMsgf(numberOfRecipesInBook > 0, TEXT("Recipe Book has no recipes. Make sure %s has recipes"), *RecipesDataTableAssetLocation))
            {
                const int randomIndex = FMath::RandRange(0, RecipeBook.Num() - 1);
                CurrentRecipeData = RecipeBook[randomIndex];

                auto first = UEnum::GetValueAsString(CurrentRecipeData->FirstIngredient);
                auto second = UEnum::GetValueAsString(CurrentRecipeData->SecondIngredient);
                auto third = UEnum::GetValueAsString(CurrentRecipeData->ThirdIngredient);
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, first);
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, second);
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, third);
                UE_LOG(LogTemp, Error, TEXT("First ingredient static %s"), *RecipesDataTableAssetLocation);
            }
        }
    }
}