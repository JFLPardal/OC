// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "RecipeData.h"

#include "Plate.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FPlateCompositionChanged, AInteractableActor const* const /*NewPlateComposition*/);

UCLASS()
class OC_API APlate : public AInteractableActor
{
	GENERATED_BODY()
public:
	APlate();
	
	void BeginPlay() override;
	void ClearPlate();
	const FRecipeData& GetRecipeData() const;
	
	FPlateCompositionChanged OnPlateCompositionChanged;
protected:
	virtual FInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
private:
	UPROPERTY(EditAnywhere)
	USceneComponent* IngredientSocket;
	
	UPROPERTY(EditAnywhere)
	FName IngredientSocketName;

	TArray<AInteractableActor*> HeldIngredients;
	FRecipeData CurrentRecipeData;
};