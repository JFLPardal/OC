// Fill out your copyright notice in the Description page of Project Settings.


#include "IngredientSpawnerCrateActor.h"

#include "Ingredient.h"

AIngredientSpawnerCrateActor::AIngredientSpawnerCrateActor()
    :AInteractableActor()
{
    InteractableType = EInteractableType::IngredientSpawnerCrate;

    IngredientSocket = CreateDefaultSubobject<USceneComponent>("IngredientSocket");
}

EInteractableInteractionOutcome AIngredientSpawnerCrateActor::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
    auto interactionOutcome = EInteractableInteractionOutcome::NoInteraction;
    if(!otherInteractable)
    {
        UE_LOG(LogTemp, Warning, TEXT("spawn ingredient"));
        GetWorld()->SpawnActor<AIngredient>(IngredientActorToSpawn, IngredientSocket->GetComponentLocation(), GetActorRotation());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Interacting with AIngredientSpawnerCrateActor while holding an interactable"));
    }
    return interactionOutcome;
}
