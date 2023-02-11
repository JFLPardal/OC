// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RecipeData.h"
#include "ActiveRecipeWidget.generated.h"

class UUIIngredientSlotWidget;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class OC_API UActiveRecipeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	void SetRecipeData(FRecipeData Recipe);
	FRecipeData GetRecipeData() const { return RecipeData; }

	UFUNCTION(BlueprintCallable)
	UUIIngredientSlotWidget* GetIngredientSlotWidget() { return IngredientSlotWidget; }

private:
	FRecipeData RecipeData;	
	UUIIngredientSlotWidget* IngredientSlotWidget;

	UPROPERTY(EditAnywhere, Category = "RecipeUI")
	UVerticalBox* IngredientsList;

	UPROPERTY(EditAnywhere, Category = "RecipeUI")
	TSubclassOf<UUIIngredientSlotWidget> IngredientSlotWidgetBP;	
};
