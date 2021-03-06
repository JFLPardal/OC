// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "RecipeData.h"

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
	const FRecipeData& GetRecipeData() const;
protected:
	virtual FInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
private:
	UPROPERTY(EditAnywhere)
	USceneComponent* IngredientSocket;
	
	AInteractableActor* HeldIngredient;
	FRecipeData CurrentRecipeData;
};