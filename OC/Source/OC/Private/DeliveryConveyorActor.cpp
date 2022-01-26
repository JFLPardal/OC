// Fill out your copyright notice in the Description page of Project Settings.


#include "DeliveryConveyorActor.h"
#include "TimerManager.h"

ADeliveryConveyorActor::ADeliveryConveyorActor()
    :AInteractableActor()
{
    InteractableType = EInteractableType::DeliveryConveyor;

    PlateSocket = CreateDefaultSubobject<USceneComponent>(TEXT("PlateSocket"));
    PlateSocket->SetupAttachment(RootComponent);
    PlateSocket->SetRelativeLocation(FVector(.0f, .0f, 70.0f));
}

void ADeliveryConveyorActor::HideAndRespawnPlate()
{
    UE_LOG(LogTemp, Warning, TEXT("Timer expired!"));
}

EInteractableInteractionOutcome ADeliveryConveyorActor::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
    EInteractableInteractionOutcome interactionOutcome = EInteractableInteractionOutcome::NoInteraction;
    if(otherInteractable)
	{
		if(otherInteractable->GetInteractableType() == EInteractableType::Plate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Interacting with Plate"));

            FAttachmentTransformRules attachmentRules(
                EAttachmentRule::SnapToTarget,
                EAttachmentRule::KeepRelative,
                EAttachmentRule::KeepWorld, 
                false);
            otherInteractable->AttachToComponent(PlateSocket, attachmentRules);

            interactionOutcome = EInteractableInteractionOutcome::ShouldDetachFromCharacter;
            
            GetWorld()->GetTimerManager().SetTimer(hideAndRespawnPlate, this, &ADeliveryConveyorActor::HideAndRespawnPlate ,.2f, false, SecondsBeforePlateRespawn);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Interacting with something for which interaction is unspecified"));
            interactionOutcome = EInteractableInteractionOutcome::NoInteraction;
		}
	}
    return interactionOutcome;
}