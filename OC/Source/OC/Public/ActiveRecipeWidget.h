// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RecipeData.h"
#include "ActiveRecipeWidget.generated.h"

class UUIIngredientSlotWidget;
class UVerticalBox;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class OC_API UActiveRecipeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetRecipeData(FRecipeData Recipe);
	FRecipeData GetRecipeData() const { return RecipeData; }

	UFUNCTION(BlueprintCallable)
	UUIIngredientSlotWidget* GetIngredientSlotWidget() { return IngredientSlotWidget; }
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox* IngredientsList;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* Show;
private:
	FRecipeData RecipeData;	
	UUIIngredientSlotWidget* IngredientSlotWidget;

	UPROPERTY(EditAnywhere, Category = "RecipeUI")
	TSubclassOf<UUIIngredientSlotWidget> IngredientSlotWidgetBP;
private:
	void PlayAnimation(UWidgetAnimation* AnimationToPlay);
};
