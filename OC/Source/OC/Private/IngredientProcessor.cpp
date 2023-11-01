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
	FInteractionOutcome outcome { EInteractableInteractionOutcome::NoInteraction };


	return outcome;
}

//// Called every frame
//void AIngredientProcessor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

