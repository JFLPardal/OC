// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestsSubsystem.h"

#include "Engine/DataTable.h"

#include "Macros.h"
#include "FRecipes.h"

URequestsSubsystem::URequestsSubsystem()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> RecipesDataTableAsset(TEXT("DataTable'/Game/DT_Recipes.DT_Recipes'"));
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
    }
}