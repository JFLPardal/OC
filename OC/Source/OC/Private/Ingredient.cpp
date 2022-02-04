// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient.h"

// Sets default values
AIngredient::AIngredient()
    :AInteractableActor()
{
	InteractableType = EInteractableType::Ingredient;
	Ingredient = EIngredient::Tomato;
}

EInteractableInteractionOutcome AIngredient::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
	UE_LOG(LogTemp, Warning, TEXT("%s interacting with Ingredient"), *GetActorLabel());
	const bool characterIsHoldingSomething = otherInteractable != nullptr;
    auto interactionOutcome = EInteractableInteractionOutcome::NoInteraction;

    if(characterIsHoldingSomething)
    {
        if(otherInteractable->GetInteractableType() == EInteractableType::Plate)
        {
            UE_LOG(LogTemp, Warning, TEXT("character is holding a plate and trying to interact with ingredient -> add ingredient to plate"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Plate interaction with something with no interaction defined"));
        }
    }
    else
    {
        interactionOutcome = EInteractableInteractionOutcome::ShouldAttachToCharacter;
    }

	return interactionOutcome;
}