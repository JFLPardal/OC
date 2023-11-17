// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "EIngredient.h"
#include "Processable.h"
#include "Ingredient.generated.h"

class UStaticMeshComponent;

UCLASS()
class OC_API AIngredient : public AInteractableActor, public IProcessable
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AIngredient();

	EIngredient GetIngredient() const;
	void SetIngredient(EIngredient IngredientToSet);

	EProcessableState GetState() const override;
	void Process() override;
	bool IsReadyToUse() const override;
protected:
	virtual FInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
private:
	UPROPERTY(EditAnywhere, Category="Ingredient")
	EIngredient Ingredient;
	
	UPROPERTY(EditAnywhere, Category = "Ingredient", meta = (ClampMin=".1", ClampMax=".9"))
	float relativeScaleForProcessedIngredient = .5f;

	UPROPERTY(VisibleAnywhere, Category = "Ingredient")
	EProcessableState ProcessableState;
};
