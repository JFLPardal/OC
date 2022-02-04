// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "IngredientSpawnerCrateActor.generated.h"

/**
 * 
 */
UCLASS()
class OC_API AIngredientSpawnerCrateActor : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	AIngredientSpawnerCrateActor();

	virtual EInteractableInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
private:
	UPROPERTY(EditAnywhere)
	USceneComponent* IngredientSocket;
};
