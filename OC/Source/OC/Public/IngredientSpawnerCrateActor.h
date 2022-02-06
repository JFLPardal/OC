// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "IngredientSpawnerCrateActor.generated.h"

class AIngredient;

/**
 * 
 */
UCLASS()
class OC_API AIngredientSpawnerCrateActor : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	AIngredientSpawnerCrateActor();

	virtual FInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
private:
	UPROPERTY(EditAnywhere)
	USceneComponent* IngredientSocket;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess="true"))
	TSubclassOf<AIngredient> IngredientActorToSpawn;

	AIngredient* SpawnedIngredient;
};
