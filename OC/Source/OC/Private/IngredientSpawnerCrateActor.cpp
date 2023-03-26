// Fill out your copyright notice in the Description page of Project Settings.


#include "IngredientSpawnerCrateActor.h"

#include "Components/TextRenderComponent.h"

#include "Ingredient.h"

FText GetTextForIngredient(EIngredient Ingredient);

AIngredientSpawnerCrateActor::AIngredientSpawnerCrateActor()
    : AInteractableActor()
    , SpawnedIngredient(nullptr)
{
    InteractableType = EInteractableType::IngredientSpawnerCrate;

    IngredientSocket = CreateDefaultSubobject<USceneComponent>("IngredientSocket");
    
    IngredientTextDisplayer = CreateDefaultSubobject<UTextRenderComponent>("IngredientTextDisplayer");
    //IngredientTextDisplayer->SetText(GetTextForIngredient(IngredientToSpawn));
    
    FVector SocketOffset{ 0.f, 0.f, 120.f };
    IngredientSocket->SetRelativeLocation(SocketOffset);
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
        if(!HasIngredientOnTop)
        {
            SpawnedIngredient = GetWorld()->SpawnActor<AIngredient>(IngredientActorToSpawn, IngredientSocket->GetComponentLocation(), GetActorRotation());
            SpawnedIngredient->SetIngredient(IngredientToSpawn);
        }
        else
        {
            interactionOutcome.Outcome = EInteractableInteractionOutcome::InteractWithOtherInteractable;
            interactionOutcome.NewActorToInteractWith = SpawnedIngredient;

            SpawnedIngredient = nullptr;
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