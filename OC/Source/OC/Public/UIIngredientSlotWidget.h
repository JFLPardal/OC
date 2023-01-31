// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EIngredient.h"

#include "UIIngredientSlotWidget.generated.h"

/**
 * 
 */

class ActiveRecipeWidgetDataSource
{
public:
	ActiveRecipeWidgetDataSource();
	ActiveRecipeWidgetDataSource(EIngredient);

	EIngredient GetIngredient() const { return Ingredient; }
private:
	EIngredient Ingredient;
};

UCLASS()
class OC_API UUIIngredientSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetDataSource(ActiveRecipeWidgetDataSource DataSourceToSet);

	UFUNCTION(BlueprintCallable)
	FText GetIngredientName() const { return IngredientName; }
	
	UPROPERTY(EditAnywhere)
	FText IngredientName;
private:
};
