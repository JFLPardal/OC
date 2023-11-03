// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticInteractableWithSocket.h"
#include "IngredientProcessor.generated.h"

UCLASS()
class OC_API AIngredientProcessor : public AStaticInteractableWithSocket
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIngredientProcessor();

protected:
	virtual FInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
private:
	bool CheckIfInteractableShouldDetachFromPlayerAndAttachToThis(FInteractionOutcome& interactionOutcome, AInteractableActor* const otherInteractable);
	bool CheckIfInteractableShouldDetachFromThisAndAttachToPlayer(FInteractionOutcome& interactionOutcome, AInteractableActor* const otherInteractable);
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
