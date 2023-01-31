// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RecipeData.h"
#include "ActiveRecipeWidget.generated.h"

class UUIIngredientSlotWidget;

/**
 * 
 */
UCLASS()
class OC_API UActiveRecipeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetRecipeData(FRecipeData Recipe);

	UFUNCTION(BlueprintCallable)
	UUIIngredientSlotWidget* GetIngredientSlotWidget() { return IngredientSlotWidget; }

	UFUNCTION(BlueprintImplementableEvent)
	void AddWidgetToHUD();
private:
	FRecipeData RecipeData;	
	UUIIngredientSlotWidget* IngredientSlotWidget;
	
	UPROPERTY(EditAnywhere, Category = "RecipeUI")
	TSubclassOf<UUIIngredientSlotWidget> IngredientSlotWidgetBP;	
};
