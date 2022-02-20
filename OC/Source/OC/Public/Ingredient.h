// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "EIngredient.h"
#include "Ingredient.generated.h"

class UStaticMeshComponent;

UCLASS()
class OC_API AIngredient : public AInteractableActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AIngredient();

	EIngredient GetIngredient() const;
protected:
	virtual FInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
private:
	UPROPERTY(EditAnywhere)
	EIngredient Ingredient;
};
