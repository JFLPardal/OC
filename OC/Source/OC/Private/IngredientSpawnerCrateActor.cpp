// Fill out your copyright notice in the Description page of Project Settings.


#include "IngredientSpawnerCrateActor.h"

#include "Components/TextRenderComponent.h"

#include "Ingredient.h"

FText GetTextForIngredient(EIngredient Ingredient);

AIngredientSpawnerCrateActor::AIngredientSpawnerCrateActor()
    : AStaticInteractableWithSocket()
    , SpawnedIngredient(nullptr)
{
    InteractableType = EInteractableType::IngredientSpawnerCrate;
    
    IngredientTextDisplayer = CreateDefaultSubobject<UTextRenderComponent>("IngredientTextDisplayer");
    //IngredientTextDisplayer->SetText(GetTextForIngredient(IngredientToSpawn));
}

void AIngredientSpawnerCrateActor::BeginPlay()
{
	Super::BeginPlay();
  
    //IngredientTextDisplayer->SetText(GetTextForIngredient(IngredientToSpawn));
}

FInteractionOutcome AIngredientSpawnerCrateActor::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
    auto interactionOutcome = FInteractionOutcome(EInteractableInteractionOutcome::NoInteraction);
    if(!otherInteractable)
    {
        const bool HasIngredientOnTop = SpawnedIngredient != nullptr;
        if(HasIngredientOnTop)
        {
            interactionOutcome.Outcome = EInteractableInteractionOutcome::InteractWithOtherInteractable;
            interactionOutcome.NewActorToInteractWith = SpawnedIngredient;

            SpawnedIngredient = nullptr;
        }
        else
        {
            SpawnedIngredient = GetWorld()->SpawnActor<AIngredient>(IngredientActorToSpawn, Socket->GetComponentLocation(), GetActorRotation());
            SpawnedIngredient->SetIngredient(IngredientToSpawn);
        }
    }
    else
    {
        if(SpawnedIngredient && otherInteractable->GetInteractableType() ==  EInteractableType::Plate)
        {
            interactionOutcome.Outcome = EInteractableInteractionOutcome::InteractWithOtherInteractable;
            interactionOutcome.NewActorToInteractWith = SpawnedIngredient;

            SpawnedIngredient = nullptr;
        }
    }
    return interactionOutcome;
}

FText GetTextForIngredient(EIngredient Ingredient)
{
    FString EnumType = FindObject<UEnum>(nullptr, TEXT("Script/OC.EIngredient"), true)->GetNameStringByValue(static_cast<uint8>(Ingredient));
    return FText::FromString(EnumType);
}