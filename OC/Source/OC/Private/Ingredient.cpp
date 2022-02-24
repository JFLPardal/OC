// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient.h"

// Sets default values
AIngredient::AIngredient()
    :AInteractableActor()
{
	InteractableType = EInteractableType::Ingredient;
	Ingredient = EIngredient::Tomato;
}
EIngredient AIngredient::GetIngredient() const
{
    return Ingredient;
}

void AIngredient::SetIngredient(EIngredient IngredientToSet)
{
    Ingredient = IngredientToSet;
}

FInteractionOutcome AIngredient::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
	const bool characterIsHoldingSomething = otherInteractable != nullptr;
    auto interactionOutcome = FInteractionOutcome(EInteractableInteractionOutcome::NoInteraction);

    if(characterIsHoldingSomething)
    {
	    UE_LOG(LogTemp, Warning, TEXT("%s interacting with Ingredient"), *(otherInteractable->GetActorLabel()));
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
        interactionOutcome.Outcome = EInteractableInteractionOutcome::ShouldAttachToCharacter;
    }

	return interactionOutcome;
}