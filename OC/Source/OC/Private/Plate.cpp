// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate.h"

APlate::APlate()
    :AInteractableActor()
{
    InteractableType = EInteractableType::Plate;
}

EInteractableInteractionOutcome APlate::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
    auto interactionOutcome = EInteractableInteractionOutcome::NoInteraction;

    if(otherInteractable->GetInteractableType() == EInteractableType::Ingredient)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s is trying to interact with plate"), *(otherInteractable->GetActorLabel()));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Plate interaction with something with no interaction defined"));
    }
    return interactionOutcome;
}

