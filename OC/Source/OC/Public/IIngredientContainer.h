// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EIngredient.h"
#include "IIngredientContainer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIIngredientContainer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OC_API IIIngredientContainer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanAssignIngredient() = 0;
	virtual void AssignIngredient(EIngredient Ingredient) = 0;
	virtual void ClearAssignedIngredient() = 0;
};
