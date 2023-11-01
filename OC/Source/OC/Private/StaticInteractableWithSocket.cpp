// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticInteractableWithSocket.h"

AStaticInteractableWithSocket::AStaticInteractableWithSocket()
    : AInteractableActor()
{
    // socket setup
    {
        Socket = CreateDefaultSubobject<USceneComponent>("Socket");
        Socket->SetupAttachment(RootComponent);
        FVector SocketOffset{ 0.f, 0.f, 70.f };
        Socket->SetRelativeLocation(SocketOffset);
    }
}

FInteractionOutcome AStaticInteractableWithSocket::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
    FInteractionOutcome interactionOutcome = FInteractionOutcome(EInteractableInteractionOutcome::NoInteraction);
    
    return interactionOutcome;
}