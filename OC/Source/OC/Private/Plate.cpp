// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate.h"

APlate::APlate()
    :AInteractableActor()
{
    InteractableType = EInteractableType::Plate;

    IngredientSocket = CreateDefaultSubobject<USceneComponent>("IngredientSocket");
    HeldIngredient = nullptr;
}

EInteractableInteractionOutcome APlate::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
    auto interactionOutcome = EInteractableInteractionOutcome::NoInteraction;
    const bool characterIsHoldingSomething = otherInteractable != nullptr;

    if(characterIsHoldingSomething)
    {
        if(otherInteractable->GetInteractableType() == EInteractableType::Ingredient)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s is trying to interact with plate"), *(otherInteractable->GetActorLabel()));
            FAttachmentTransformRules attachmentRules(
                    EAttachmentRule::SnapToTarget,
                    EAttachmentRule::KeepRelative,
                    EAttachmentRule::KeepWorld, 
                    false);
            otherInteractable->AttachToComponent(IngredientSocket, attachmentRules);
            HeldIngredient = otherInteractable;
            interactionOutcome = EInteractableInteractionOutcome::ShouldDetachFromCharacter;
            auto MeshComponent = HeldIngredient->FindComponentByClass<UStaticMeshComponent>();
            if(MeshComponent)
            {
                MeshComponent->SetGenerateOverlapEvents(false);
            }
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

void APlate::ClearPlate()
{
    if(HeldIngredient)
    {
        FDetachmentTransformRules dettachmentRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative,EDetachmentRule::KeepWorld, false);
        HeldIngredient->DetachFromActor(dettachmentRules);
        HeldIngredient = nullptr;
    }

    IngredientSocket = nullptr;
}