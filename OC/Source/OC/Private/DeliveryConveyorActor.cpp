// Fill out your copyright notice in the Description page of Project Settings.


#include "DeliveryConveyorActor.h"

#include "Plate.h"
#include "TimerManager.h"

ADeliveryConveyorActor::ADeliveryConveyorActor()
    :AInteractableActor()
{
    InteractableType = EInteractableType::DeliveryConveyor;

    PlateSocket = CreateDefaultSubobject<USceneComponent>(TEXT("PlateSocket"));
    checkfSlow(PlateSocket, TEXT("PlateSocket not created for %s"), *GetActorLabel());
    PlateSocket->SetupAttachment(RootComponent);
    PlateSocket->SetRelativeLocation(FVector(.0f, .0f, 70.0f));
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
            if(OnPlateDelivered.IsBound())
            {
                OnPlateDelivered.Broadcast(EIngredient::Beans);
            }
            UE_LOG(LogTemp, Warning, TEXT("Interacting with Plate"));

            FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld, false);
            otherInteractable->AttachToComponent(PlateSocket, attachmentRules);
            HeldPlate = Cast<APlate>(otherInteractable);
            if(!HeldPlate)
            {
                UE_LOG(LogTemp, Error, TEXT("[ADeliveryConveyorActor] - couldn't convert %s to APlate during interaction"), *otherInteractable->GetActorLabel());
            }
            interactionOutcome.Outcome = EInteractableInteractionOutcome::ShouldDetachFromCharacter;
            GetWorld()->GetTimerManager().SetTimer(hideAndRespawnPlate, this, &ADeliveryConveyorActor::HideAndRespawnPlate ,.2f, false, SecondsBeforePlateRespawn);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Interacting with something for which interaction is unspecified"));
            interactionOutcome.Outcome = EInteractableInteractionOutcome::NoInteraction;
        }
    }

    return interactionOutcome;
}