// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IIngredientContainer.h"
#include "OCUWIngredientBalloonContainer.generated.h"

/**
 * 
 */

class UImage;

UCLASS()
class OC_API UOCUWIngredientBalloonContainer : public UUserWidget, public IIIngredientContainer
{
	GENERATED_BODY()
public:
	bool CanAssignIngredient() override;
	void AssignIngredient(EIngredient Ingredient) override;
	void ClearAssignedIngredient() override;
protected:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Representation;
private:
	EIngredient AssignedIngredient = EIngredient::Invalid;
};
