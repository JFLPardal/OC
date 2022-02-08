// Fill out your copyright notice in the Description page of Project Settings.


#include "IngredientSpawnerCrateActor.h"

#include "Ingredient.h"

AIngredientSpawnerCrateActor::AIngredientSpawnerCrateActor()
    : AInteractableActor()
    , SpawnedIngredient(nullptr)
{
    InteractableType = EInteractableType::IngredientSpawnerCrate;

    IngredientSocket = CreateDefaultSubobject<USceneComponent>("IngredientSocket");
    FVector SocketOffset{ 0.f, 0.f, 120.f };
    IngredientSocket->SetRelativeLocation(SocketOffset);
}

FInteractionOutcome AIngredientSpawnerCrateActor::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
    auto interactionOutcome = FInteractionOutcome(EInteractableInteractionOutcome::NoInteraction);
    if(!otherInteractable)
    {
        const bool HasIngredientOnTop = SpawnedIngredient != nullptr;
        if(!HasIngredientOnTop)
        {
            UE_LOG(LogTemp, Warning, TEXT("spawn ingredient"));

            SpawnedIngredient = GetWorld()->SpawnActor<AIngredient>(IngredientActorToSpawn, IngredientSocket->GetComponentLocation(), GetActorRotation());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("attach ingredient to chracter"));

            interactionOutcome.Outcome = EInteractableInteractionOutcome::InteractWithOtherInteractable;
            interactionOutcome.NewActorToInteractWith = SpawnedIngredient;

            SpawnedIngredient = nullptr;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Interacting with AIngredientSpawnerCrateActor while holding an interactable"));
        if(SpawnedIngredient && otherInteractable->GetInteractableType() ==  EInteractableType::Plate)
        {
            interactionOutcome.Outcome = EInteractableInteractionOutcome::InteractWithOtherInteractable;
            interactionOutcome.NewActorToInteractWith = SpawnedIngredient;

            SpawnedIngredient = nullptr;
        }
    }
    return interactionOutcome;
}
