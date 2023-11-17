// Fill out your copyright notice in the Description page of Project Settings.


#include "DeliveryConveyorActor.h"

#include "Plate.h"
#include "TimerManager.h"

ADeliveryConveyorActor::ADeliveryConveyorActor()
    : AStaticInteractableWithSocket()
{
    InteractionOptions = EInteractableInteractionOptions::CanOnlyAttach;
    InteractableType = EInteractableType::DeliveryConveyor;

    SetSpecificInteractionCallbacks();
}

void ADeliveryConveyorActor::HideAndRespawnPlate()
{
    APlate* HeldPlate = Cast<APlate>(InteractableInSocket);
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
            InteractableInSocket = nullptr;
        }
    }
}

void ADeliveryConveyorActor::InteractWithPlate(AInteractableActor* const otherInteractable)
{
    APlate* Plate = Cast<APlate>(otherInteractable);
    if (Plate)
    {
        if (ensureMsgf(OnPlateDelivered.IsBound(), TEXT("OnPlateDelivered not bound")))
        {
            OnPlateDelivered.Broadcast(Cast<APlate>(otherInteractable));
        }

        Plate->DisableInteraction();
        if (ensureMsgf(Plate, TEXT("[ADeliveryConveyorActor] - couldn't convert %s to APlate during interaction"), *otherInteractable->GetActorLabel()))
        {
            GetWorld()->GetTimerManager().SetTimer(hideAndRespawnPlate, this, &ADeliveryConveyorActor::HideAndRespawnPlate, .2f, false, SecondsBeforePlateRespawn);
        }
    }
}

void ADeliveryConveyorActor::SetSpecificInteractionCallbacks()
{
    auto PlateInteractionCallback = [this](AInteractableActor* const interactable)
        {
            if (this)
            {
                InteractWithPlate(interactable);
            }
        };
    InteractableTypeToInteraction.Emplace(EInteractableType::Plate, PlateInteractionCallback);
}