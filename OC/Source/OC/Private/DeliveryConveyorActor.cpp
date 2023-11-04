// Fill out your copyright notice in the Description page of Project Settings.


#include "DeliveryConveyorActor.h"

#include "Plate.h"
#include "TimerManager.h"

ADeliveryConveyorActor::ADeliveryConveyorActor()
    :AStaticInteractableWithSocket()
{
    InteractableType = EInteractableType::DeliveryConveyor;
}

void ADeliveryConveyorActor::HideAndRespawnPlate()
{
    if(HeldPlate)
    {
        if(ensureMsgf(PlateRespawnLocation, TEXT("PlateRespawnLocation not set for %s"), *GetActorLabel()))
        {
            FDetachmentTransformRules dettachmentRules(
                    EDetachmentRule::KeepWorld,
                    EDetachmentRule::KeepRelative,
                    EDetachmentRule::KeepWorld, 
                    false);
            HeldPlate->ClearPlate();
            HeldPlate->SetActorLocation(PlateRespawnLocation->GetActorLocation());
            HeldPlate->DetachFromActor(dettachmentRules);
            HeldPlate = nullptr;
        }
    }
}

FInteractionOutcome ADeliveryConveyorActor::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
    auto interactionOutcome = FInteractionOutcome(EInteractableInteractionOutcome::NoInteraction);
    const bool characterIsHoldingSomething = otherInteractable != nullptr;

    if(characterIsHoldingSomething)
    {
        if(otherInteractable->GetInteractableType() == EInteractableType::Plate)
        {
            if(ensureMsgf(OnPlateDelivered.IsBound(), TEXT("OnPlateDelivered not bound")))
            {
                OnPlateDelivered.Broadcast(Cast<APlate>(otherInteractable));
            }

            FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld, false);
            otherInteractable->AttachToComponent(Socket, attachmentRules);
            HeldPlate = Cast<APlate>(otherInteractable);
            HeldPlate->DisableInteraction();
            if(ensureMsgf(HeldPlate, TEXT("[ADeliveryConveyorActor] - couldn't convert %s to APlate during interaction"), *otherInteractable->GetActorLabel()))
            {
                interactionOutcome.Outcome = EInteractableInteractionOutcome::ShouldDetachFromCharacter;
                GetWorld()->GetTimerManager().SetTimer(hideAndRespawnPlate, this, &ADeliveryConveyorActor::HideAndRespawnPlate ,.2f, false, SecondsBeforePlateRespawn);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Interacting with something for which interaction is unspecified"));
            interactionOutcome.Outcome = EInteractableInteractionOutcome::NoInteraction;
        }
    }

    return interactionOutcome;
}