// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestsSubsystem.h"

#include "Engine/DataTable.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/Class.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "Macros.h"
#include "FRecipes.h"
#include "Plate.h"
#include "RecipeData.h"

URequestsSubsystem::URequestsSubsystem()
{
    //RecipesDataTableAssetLocation = "DataTable'/Game/DT_Recipes.DT_Recipes'";
    RecipesDataTableAssetLocation = "DataTable'/Game/DT_Recipes_Simple.DT_Recipes_Simple'";
    
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
    // subscribe to the OnPlateDelivered events
    {
        if(UWorld* World = GetWorld())
        {
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeliveryConveyorActor::StaticClass(), DeliveryConveyorActors);
            if(ensureMsgf(DeliveryConveyorActors.Num() > 0, TEXT("No DeliveryConveyorActors found in World, delivered plates will not be checked for correct recipes")))
            {
                for(AActor* deliveryConveyorActor : DeliveryConveyorActors )
                {
                    Cast<ADeliveryConveyorActor>(deliveryConveyorActor)->OnPlateDelivered.AddDynamic(this, &URequestsSubsystem::CheckIfPlateHasActiveRecipe);
                    DTOS("subscribed to CheckIfPlateHasActiveRecipe")
                }
            }
        }
        else
        {
            DTOS("URequestsSubsystem::URequestsSubsystem - World not found")
        }
    }

    // set timer to call GenerateRecipe
    {
        const bool ShouldRepeat = false;
        const float SecondsBeforeGeneratingFirstRecipe = 0.5f;
        const float SecondsBeforeGeneratingSubsequentRecipes = 5.0f;
        GetWorld()->GetTimerManager().SetTimer(GenerateRecipeTimer, this, &URequestsSubsystem::GenerateRecipe, SecondsBeforeGeneratingSubsequentRecipes, ShouldRepeat, SecondsBeforeGeneratingFirstRecipe);
    }
}

void URequestsSubsystem::GenerateRecipe()
{
    if(ensureMsgf(RecipesDataTable, TEXT("URequestsSubsystem - RecipedDataTable is empty")))
    {
        int index = 0;
        ActiveRecipesData.InsertDefaulted(index, maxNumberOfSimultaneousActiveRecipes);

        // this implies that we will have maxNumberOfSimultaneousActiveRecipes from the start of the level
        for(int i = 0; i < ActiveRecipesData.Num(); ++i)
        {
            ActiveRecipesData[i] = GetRandomRecipeFromRecipeBook();
            OnGeneratedNewRequest.Broadcast(*ActiveRecipesData[i]);
        }
        //OC::PrintRecipe(*ActiveRecipeData, OC::PrintTo::outputAndScreen);
    }
}

void URequestsSubsystem::CheckIfPlateHasActiveRecipe(APlate* Plate)
{
    DTOS("checking if plate has a matching recipe");
    if(ensureMsgf(Plate, TEXT("Plate passed as parameter for OnPlateDelivered event is a nullptr")))
    {
        auto RecipeData = Plate->GetRecipeData();
        OnCompletedRequest.Broadcast(RecipeData);

        FString PlateRecipe = RecipeData.RecipeIngredients.ToString();
        UE_LOG(LogTemp, Warning, TEXT("Plate ingredients in Request %s"), *PlateRecipe);

        const auto PlateIngredients = RecipeData.RecipeIngredients;

        for(auto ActiveRecipe : ActiveRecipesData)
        {
            const auto ActiveRecipeIngredients = ActiveRecipe->RecipeIngredients;

            const bool PlateHasValidRecipe = ((PlateIngredients | ActiveRecipeIngredients) == ActiveRecipeIngredients) 
                                           && (PlateIngredients != TStaticBitArray<16>(0));
            if(PlateHasValidRecipe)
            {
                DTOS("VALID RECIPE");
            }
            else
            {
                DTOS("INVALID RECIPE");
            }
        }
    }
}

TArray<EIngredient> URequestsSubsystem::GetIngredientsList(FRecipeData recipeData) const
{
    TArray<EIngredient> IngredientList{};

    IngredientList.Add(EIngredient::Tomato);

    return IngredientList;
}

FRecipeData* URequestsSubsystem::GetRandomRecipeFromRecipeBook()
{
    FRecipeData* RecipeData = new FRecipeData(); // is this leaking? I'm pretty sure it is!

    auto ConvertRecipeBookEntryToRecipeData = [RecipeData](FRecipes* Recipe)
    {
        auto AddIngredientToRecipeIfIngredientIsValid = [RecipeData](EIngredient Ingredient)
        {
            const bool IngredientIsValid = Ingredient != EIngredient::Invalid;
            if(IngredientIsValid)
            {
                RecipeData->AddIngredient(Ingredient);
            }
        };

        if(Recipe)
        {
            AddIngredientToRecipeIfIngredientIsValid(Recipe->FirstIngredient);
            AddIngredientToRecipeIfIngredientIsValid(Recipe->SecondIngredient);
            AddIngredientToRecipeIfIngredientIsValid(Recipe->ThirdIngredient);
        }
    };

    const auto numberOfRecipesInBook = RecipeBook.Num();
    if(ensureMsgf(numberOfRecipesInBook > 0, TEXT("Recipe Book has no recipes. Make sure %s has recipes"), *RecipesDataTableAssetLocation))
    {
        const int randomIndex = FMath::RandRange(0, RecipeBook.Num() - 1);
        FRecipes* Recipe = RecipeBook[randomIndex];
        ConvertRecipeBookEntryToRecipeData(Recipe);
    }

    FString ActiveRecipe = RecipeData->RecipeIngredients.ToString();
    UE_LOG(LogTemp, Warning, TEXT("Active Recipe %s"), *ActiveRecipe);

    return RecipeData;
}