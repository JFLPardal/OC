// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractableActor.h"
#include "EIngredient.h"

#include "IngredientSpawnerCrateActor.generated.h"

class UTextRenderComponent;
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

	virtual void BeginPlay() override;

	virtual FInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"), Category="TextRenderer")
	UTextRenderComponent* IngredientTextDisplayer;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"), Category="Ingredient")
	USceneComponent* IngredientSocket;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"), Category="Ingredient")
	TSubclassOf<AIngredient> IngredientActorToSpawn;

	UPROPERTY(EditAnywhere, Category="Ingredient")
	EIngredient IngredientToSpawn;

	AIngredient* SpawnedIngredient;
};
