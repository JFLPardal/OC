// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IIngredientContainer.h"
#include "OCUWIngredientBalloonContainer.generated.h"

/**
 * 
 */
UCLASS()
class OC_API UOCUWIngredientBalloonContainer : public UUserWidget, public IIIngredientContainer
{
	GENERATED_BODY()
public:
	bool CanAssignIngredient() override;
	void AssignIngredient(EIngredient Ingredient) override;
private:
	EIngredient AssignedIngredient = EIngredient::Invalid;
};
