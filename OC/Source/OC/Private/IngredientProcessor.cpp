// Fill out your copyright notice in the Description page of Project Settings.


#include "IngredientProcessor.h"

#include "Processable.h"

AIngredientProcessor::AIngredientProcessor()
	:AStaticInteractableWithSocket()
{
	InteractableType = EInteractableType::IngredientProcessor;
}

EUsageOutcome AIngredientProcessor::TryToUse()
{
	EUsageOutcome Outcome{ EUsageOutcome::FailedToUse };
	if (CanUse())
	{
		IProcessable* Processable = Cast<IProcessable>(InteractableInSocket);
		Processable->Process();
		EProcessableState ProcessableState = Processable->GetState();

		switch (ProcessableState)
		{
		case EProcessableState::NotProcessed:
		case EProcessableState::PartiallyProcessed:
			Outcome = EUsageOutcome::NotFullyProcessed;
			break;
		case EProcessableState::FullyProcessed:
			Outcome = EUsageOutcome::FullyProcessed;
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("missing EProcessableState implementation"));
			break;
		}
	}
	return Outcome;
}

bool AIngredientProcessor::CanUse() const
{
	bool IsInteractableInSocketReadyToUse = false;
	if (IProcessable const* const InteractableInSocketIsProcessable = Cast<IProcessable>(InteractableInSocket))
	{
		IsInteractableInSocketReadyToUse = InteractableInSocketIsProcessable->IsReadyToUse();
	}

	return HasInteractableInSocket() && !IsInteractableInSocketReadyToUse;
}