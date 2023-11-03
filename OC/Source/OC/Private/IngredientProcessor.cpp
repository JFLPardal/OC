// Fill out your copyright notice in the Description page of Project Settings.


#include "IngredientProcessor.h"

// Sets default values
AIngredientProcessor::AIngredientProcessor()
	:AStaticInteractableWithSocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	InteractableType = EInteractableType::IngredientProcessor;
}

FInteractionOutcome AIngredientProcessor::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
	FInteractionOutcome interactionOutcome { EInteractableInteractionOutcome::NoInteraction };

	if (CheckIfInteractableShouldDetachFromPlayerAndAttachToThis(interactionOutcome, otherInteractable))
	{
		return interactionOutcome;
	}
	
	if (CheckIfInteractableShouldDetachFromThisAndAttachToPlayer(interactionOutcome, otherInteractable))
	{
		return interactionOutcome;
	}

	return interactionOutcome;
}

bool AIngredientProcessor::CheckIfInteractableShouldDetachFromPlayerAndAttachToThis(FInteractionOutcome& interactionOutcome, AInteractableActor* const otherInteractable)
{
	bool const interactableShouldDetachFromPlayerAndAttachToThis = otherInteractable && !InteractableInSocket && Socket;

	if (interactableShouldDetachFromPlayerAndAttachToThis)
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

	return interactionOutcome.Outcome != EInteractableInteractionOutcome::NoInteraction;
}

bool AIngredientProcessor::CheckIfInteractableShouldDetachFromThisAndAttachToPlayer(FInteractionOutcome& interactionOutcome, AInteractableActor* const otherInteractable)
{
	bool const interactableShouldDetachFromThisAndAttachToPlayer = !otherInteractable && InteractableInSocket && Socket;
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

//// Called every frame
//void AIngredientProcessor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

