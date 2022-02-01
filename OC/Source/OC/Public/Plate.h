// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Plate.generated.h"

/**
 * 
 */
UCLASS()
class OC_API APlate : public AInteractableActor
{
	GENERATED_BODY()
public:
	APlate();
	
	void ClearPlate();
protected:
	virtual EInteractableInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* IngredientSocket;
	
	AInteractableActor* HeldIngredient;
};
