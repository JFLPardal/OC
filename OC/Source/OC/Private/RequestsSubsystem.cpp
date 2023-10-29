// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestsSubsystem.h"

#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/Class.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

#include "CustomCVars.h"
#include "FRecipes.h"
#include "Macros.h"
#include "Plate.h"
#include "RecipeData.h"
#include "RequestSubsystemPOD.h"

namespace Requests
{
    FString SimpleRecipesDataTableAssetLocation{ "DataTable'/Game/DT_Recipes_Simple.DT_Recipes_Simple'" };
    FString DefaultRecipesDataTableAssetLocation{ "DataTable'/Game/DT_Recipes.DT_Recipes'" };
    FString RequestSubsystemPODLocation{ "RequestSubsystemPOD'/Game/DA_RequestSubsystemPOD.DA_RequestSubsystemPOD'" };
}


URequestsSubsystem::URequestsSubsystem()
{
    SetInitData();
    CreateRecipesDataTable();
    
    maxNumberOfSimultaneousActiveRecipes = static_cast<uint8>(CVarMaxNumberOfSimultaneousActiveRecipes->GetInt());
}

void URequestsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{    
    Super::Initialize(Collection);
    // subscribe to the OnPlateDelivered events
    {
        if(UWorld* World = GetWorld())
        {
            TimerManager = &(GetWorld()->GetTimerManager());

            UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeliveryConveyorActor::StaticClass(), DeliveryConveyorActors);
            if(ensureMsgf(DeliveryConveyorActors.Num() > 0, TEXT("No DeliveryConveyorActors found in World, delivered plates will not be checked for correct recipes")))
            {
                for(AActor* deliveryConveyorActor : DeliveryConveyorActors )
                {
                    Cast<ADeliveryConveyorActor>(deliveryConveyorActor)->OnPlateDelivered.AddDynamic(this, &URequestsSubsystem::CheckIfPlateHasActiveRecipe);
                }
            }
        }
    }

    ActiveRecipes.Reserve(maxNumberOfSimultaneousActiveRecipes);
    // set timer to call GenerateRecipe
    {
        if (ensureMsgf(InitData != nullptr, TEXT("InitData in RequestSubsystem was not initialized, requests will not be generated")))
        {
            bool const bIsFirstTimeGenerating = true;
            StartRequestGeneration(bIsFirstTimeGenerating);
        }
    }
}

void URequestsSubsystem::GenerateRecipe()
{
    int numberOfActiveRecipes = ActiveRecipes.Num();
    if(numberOfActiveRecipes < maxNumberOfSimultaneousActiveRecipes)    
    {
        if (ensureMsgf(RecipesDataTable, TEXT("URequestsSubsystem - RecipedDataTable is empty - are you sure the path is correct?")))
        {
            TSharedPtr<FRecipeData> newRecipe = GetSharedPtrToRandomRecipeFromRecipeBook();
            ActiveRecipes.Add(newRecipe);

            OnGeneratedNewRequest.Broadcast(*newRecipe);

            // check if GenerateRecipeTimer should be paused
            {
                bool const shouldPauseGeneratingRequests = ActiveRecipes.Num() >= maxNumberOfSimultaneousActiveRecipes;
                if (shouldPauseGeneratingRequests)
                {
                    PauseRequestGeneration();
                }
            }
        }
    }
}

void URequestsSubsystem::CheckIfPlateHasActiveRecipe(APlate* Plate)
{
    if(ensureMsgf(Plate, TEXT("Plate passed as parameter for OnPlateDelivered event is a nullptr")))
    {
        auto RecipeData = Plate->GetRecipeData();
        OnCompletedRequest.Broadcast(RecipeData);

        FString PlateRecipe = RecipeData.RecipeIngredients.ToString();

        const auto PlateIngredients = RecipeData.RecipeIngredients;

        int const invalidIndex = -1;
        int32 indexOfCompletedRecipe = invalidIndex;
        for(auto ActiveRecipe : ActiveRecipes)
        {
            const auto ActiveRecipeIngredients = ActiveRecipe->RecipeIngredients;

            const bool PlateHasValidRecipe = ((PlateIngredients | ActiveRecipeIngredients) == ActiveRecipeIngredients) 
                                           && (PlateIngredients != TStaticBitArray<16>(0));
            
            if(PlateHasValidRecipe)
            {
                indexOfCompletedRecipe = ActiveRecipes.IndexOfByKey(ActiveRecipe);
                break;
            }
            else
            {
                DTOS("INVALID RECIPE");
            }
        }

        // what happens if we generate a recipe before this function is done processing? Is that possible?
        bool const shouldDeleteRecipe = indexOfCompletedRecipe != invalidIndex;
        if (shouldDeleteRecipe)
        {
            ActiveRecipes.RemoveAt(indexOfCompletedRecipe);
            if (IsRequestGenerationPaused())
            {
                ResumeRequestGeneration();
            }
        }
    }
}

bool URequestsSubsystem::IsRequestGenerationPaused()
{
    return !TimerManager->IsTimerActive(GenerateRecipeTimer);
}

void URequestsSubsystem::StartRequestGeneration(bool bIsFirstTimeGenerating)
{
    const bool ShouldRepeat = true;
    const float SecondsBeforeGeneratingSubsequentRecipes = InitData->SecondsBeforeGeneratingSubsequentRequests;
    const float SecondsBeforeGeneratingFirstRecipe = (bIsFirstTimeGenerating) ? InitData->SecondsBeforeGeneratingFirstRecipe : SecondsBeforeGeneratingSubsequentRecipes;

    TimerManager->SetTimer(
        GenerateRecipeTimer,
        this,
        &URequestsSubsystem::GenerateRecipe,
        SecondsBeforeGeneratingSubsequentRecipes,
        ShouldRepeat,
        SecondsBeforeGeneratingFirstRecipe);
}

void URequestsSubsystem::ResumeRequestGeneration()
{
    bool const bIsFirstTimeGenerating = false;
    StartRequestGeneration(bIsFirstTimeGenerating);
}

void URequestsSubsystem::PauseRequestGeneration()
{
    TimerManager->ClearTimer(GenerateRecipeTimer);
}

TArray<EIngredient> URequestsSubsystem::GetIngredientsList(const FRecipeData& recipeData) const
{
    return recipeData.GetIngredients();
}

TSharedPtr<FRecipeData> URequestsSubsystem::GetSharedPtrToRandomRecipeFromRecipeBook()
{
    TSharedPtr<FRecipeData> RecipeData = MakeShared<FRecipeData>();

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

    return RecipeData;
}

void URequestsSubsystem::DebugGenerateNewRequest()
{
    GenerateRecipe();
}

void URequestsSubsystem::DebugCompleteOldestRequest()
{
    if (ActiveRecipes.Num() > 0)
    {
        FRecipeData OldestRecipe = *ActiveRecipes[0];
        OnCompletedRequest.Broadcast(OldestRecipe);
        ActiveRecipes.RemoveAt(0);
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
                + GetStringValue(Recipe.SecondIngredient) + "\n"
                + GetStringValue(Recipe.ThirdIngredient);
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FullRecipe);
        };

        using namespace OC;
        switch (printLocation)
        {
        case PrintTo::outputOnly:
        {
            PrintToOutput(Recipe);
            break;
        }
        case PrintTo::screenOnly:
        {
            PrintToScreen(Recipe);
            break;
        }
        case PrintTo::outputAndScreen:
        {
            PrintToOutput(Recipe);
            PrintToScreen(Recipe);
            break;
        }
        }
    }
}

void URequestsSubsystem::CreateRecipesDataTable()
{
    RecipesDataTableAssetLocation = CVarShouldUseSimpleRecipeBook->GetBool() ?
        Requests::SimpleRecipesDataTableAssetLocation :
        Requests::DefaultRecipesDataTableAssetLocation;

    ConstructorHelpers::FObjectFinder<UDataTable> RecipesDataTableAsset = ConstructorHelpers::FObjectFinder<UDataTable>(*RecipesDataTableAssetLocation);

    if (ensureMsgf(RecipesDataTableAsset.Succeeded(), TEXT("RecipedDataTable %s not found - check the recipes' data table name"), *RecipesDataTableAssetLocation))
    {
        RecipesDataTable = RecipesDataTableAsset.Object;

        // Cache all recipes
        const FString contextString{ TEXT("RequestsSubsystem - RecipesDataTable") };
        RecipesDataTable->GetAllRows<FRecipes>(contextString, RecipeBook);
    }
}

void URequestsSubsystem::SetInitData()
{
    ConstructorHelpers::FObjectFinder<URequestSubsystemPOD> InitializerDataAsset = ConstructorHelpers::FObjectFinder<URequestSubsystemPOD>(*Requests::RequestSubsystemPODLocation);
    if (ensureMsgf(InitializerDataAsset.Succeeded(), TEXT("RequestSubsystemPOD %s not found - check the RequestSubsystemPOD's location"), *Requests::RequestSubsystemPODLocation))
    {
        InitData = InitializerDataAsset.Object;
    }
}
