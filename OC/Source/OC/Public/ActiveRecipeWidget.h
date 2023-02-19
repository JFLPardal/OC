// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RecipeData.h"
#include "ActiveRecipeWidget.generated.h"

class UUIIngredientSlotWidget;
class UVerticalBox;
class UWidgetAnimation;

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemovedRequestWidgetFromHUD, FRecipeData, RecipeData);

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddedRequestWidgetToHUD, FRecipeData, RecipeData);

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
	FRecipeData GetRecipeData() const { return RecipeData; }

	UFUNCTION(BlueprintCallable)
	UUIIngredientSlotWidget* GetIngredientSlotWidget() { return IngredientSlotWidget; }

	void Completed();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FRemovedRequestWidgetFromHUD OnRemovedRequestWidgetFromHUD;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FAddedRequestWidgetToHUD OnAddedRequestWidgetToHUD;
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox* IngredientsList;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* Show;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly)
	UWidgetAnimation* Hide;
protected:
	virtual void OnAnimationFinished_Implementation(UWidgetAnimation const* Animation) override;

	UFUNCTION()
	void FinishedPlayingHideAnimation();
private:
	FRecipeData RecipeData;	
	UUIIngredientSlotWidget* IngredientSlotWidget;

	UPROPERTY(EditAnywhere, Category = "RecipeUI")
	TSubclassOf<UUIIngredientSlotWidget> IngredientSlotWidgetBP;

	UPROPERTY(EditAnywhere, Category = "RecipeUI")
	float AnimationsPlaybackSpeed = 1.0f;
private:
	void PlayAnimation(UWidgetAnimation* AnimationToPlay);
};
