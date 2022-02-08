// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate.h"

APlate::APlate()
    :AInteractableActor()
{
    InteractableType = EInteractableType::Plate;

    IngredientSocket = CreateDefaultSubobject<USceneComponent>("IngredientSocket");
    HeldIngredient = nullptr;
}

FInteractionOutcome APlate::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
    auto interactionOutcome = FInteractionOutcome(EInteractableInteractionOutcome::NoInteraction);
    const bool characterIsHoldingSomething = otherInteractable != nullptr;

    if(characterIsHoldingSomething)
    {
        if(otherInteractable->GetInteractableType() == EInteractableType::Ingredient)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s is trying to interact with plate"), *(otherInteractable->GetActorLabel()));
            
            HeldIngredient = otherInteractable;

            FAttachmentTransformRules attachmentRules( EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false);
            HeldIngredient->AttachToComponent(IngredientSocket, attachmentRules);
            
            auto MeshComponent = HeldIngredient->FindComponentByClass<UStaticMeshComponent>();
            if(MeshComponent)
            {
                MeshComponent->SetGenerateOverlapEvents(false);
            }
            
            interactionOutcome.Outcome = EInteractableInteractionOutcome::ShouldDetachFromCharacter;
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

void APlate::ClearPlate()
{
    if(HeldIngredient)
    {
        HeldIngredient->Destroy();
        HeldIngredient = nullptr;
    }
}