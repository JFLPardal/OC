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
	return EInteractableInteractionOutcome::NoInteraction;
}