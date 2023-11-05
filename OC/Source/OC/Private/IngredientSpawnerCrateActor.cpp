// Fill out your copyright notice in the Description page of Project Settings.


#include "IngredientSpawnerCrateActor.h"

#include "Components/TextRenderComponent.h"

#include "Ingredient.h"

FText GetTextForIngredient(EIngredient Ingredient);

AIngredientSpawnerCrateActor::AIngredientSpawnerCrateActor()
    : AStaticInteractableWithSocket()
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
    FInteractionOutcome interactionOutcome = Super::AttemptInteractionWith(otherInteractable);
    
    if(interactionOutcome.Outcome == EInteractableInteractionOutcome::NoInteraction)
    {
        if(!otherInteractable)
        {
            if(!HasInteractableInSocket())
            {
                AIngredient* const SpawnedIngredient = GetWorld()->SpawnActor<AIngredient>(IngredientActorToSpawn, Socket->GetComponentLocation(), GetActorRotation());
                SpawnedIngredient->SetIngredient(IngredientToSpawn);
                InteractableInSocket = SpawnedIngredient;
            }
        }
    }

    return interactionOutcome;
}

FText GetTextForIngredient(EIngredient Ingredient)
{
    FString EnumType = FindObject<UEnum>(nullptr, TEXT("Script/OC.EIngredient"), true)->GetNameStringByValue(static_cast<uint8>(Ingredient));
    return FText::FromString(EnumType);
}