// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticInteractableWithSocket.h"

AStaticInteractableWithSocket::AStaticInteractableWithSocket()
    : AInteractableActor()
	, InteractionOptions(EInteractableInteractionOptions::Default)
    , InteractableInSocket(nullptr)
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
    
	if (CheckIfInteractableShouldAttachToThisAndDetachFromPlayer(interactionOutcome, otherInteractable))
	{
		return interactionOutcome;
	}

	if (CheckIfInteractableShouldDetachFromThisAndAttachToPlayer(interactionOutcome, otherInteractable))
	{
		return interactionOutcome;
	}

    return interactionOutcome;
}

bool AStaticInteractableWithSocket::CheckIfInteractableShouldAttachToThisAndDetachFromPlayer(FInteractionOutcome& interactionOutcome, AInteractableActor* const otherInteractable)
{
	bool interactableShouldAttachToThisAndDetachFromPlayer = CanAttachInteractable() && otherInteractable && !InteractableInSocket && Socket;

	if (interactableShouldAttachToThisAndDetachFromPlayer)
	{
		bool const restrictionsOnInteraction = InteractableTypeToInteraction.Num() > 0;
		if (restrictionsOnInteraction)
		{
			auto InteractionToExecute = InteractableTypeToInteraction.Find(otherInteractable->GetInteractableType());
			if (InteractionToExecute != nullptr)
			{
				(*InteractionToExecute)(otherInteractable);
			}
			else
			{
				interactableShouldAttachToThisAndDetachFromPlayer = false;
			}
		}

		if (interactableShouldAttachToThisAndDetachFromPlayer)
		{
			bool const WeldSimulatedBodies = false;
			FAttachmentTransformRules attachmentRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepRelative,
				EAttachmentRule::KeepWorld,
				WeldSimulatedBodies);
			otherInteractable->AttachToComponent(Socket, attachmentRules);

			InteractableInSocket = otherInteractable;

			interactionOutcome.Outcome = EInteractableInteractionOutcome::ShouldDetachFromCharacter;
		}
	}

	return interactionOutcome.Outcome != EInteractableInteractionOutcome::NoInteraction;
}

bool AStaticInteractableWithSocket::CheckIfInteractableShouldDetachFromThisAndAttachToPlayer(FInteractionOutcome& interactionOutcome, AInteractableActor* const otherInteractable)
{
	bool const interactableShouldDetachFromThisAndAttachToPlayer = CanDetachInteractable() && !otherInteractable && InteractableInSocket && Socket;
	if (interactableShouldDetachFromThisAndAttachToPlayer)
	{
		bool const WeldSimulatedBodies = false;
		FDetachmentTransformRules dettachmentRules(
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepRelative,
			EDetachmentRule::KeepWorld,
			WeldSimulatedBodies);

		interactionOutcome.Outcome = EInteractableInteractionOutcome::InteractWithOtherInteractable;
		interactionOutcome.NewActorToInteractWith = InteractableInSocket;

		InteractableInSocket->DetachFromActor(dettachmentRules);
		InteractableInSocket = nullptr;
	}

	return interactionOutcome.Outcome != EInteractableInteractionOutcome::NoInteraction;
}

bool AStaticInteractableWithSocket::CanAttachInteractable() const
{
	return InteractionOptions == EInteractableInteractionOptions::CanAttachAndDetach ||
		InteractionOptions == EInteractableInteractionOptions::CanOnlyAttach;
}

bool AStaticInteractableWithSocket::CanDetachInteractable() const
{
	return InteractionOptions == EInteractableInteractionOptions::CanAttachAndDetach ||
		InteractionOptions == EInteractableInteractionOptions::CanOnlyDetach;
}

bool AStaticInteractableWithSocket::HasInteractableInSocket() const
{
	return InteractableInSocket != nullptr;
}
