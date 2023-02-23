// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecipeData.h"

#include "UOCUWActiveRequestsHUD.generated.h"

class UActiveRecipeWidget;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class OC_API UUOCUWActiveRequestsHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
protected:
	UFUNCTION()
	void OnRequestWidgetCreated(UActiveRecipeWidget* CreatedWidget);

	UFUNCTION()
	void OnFinishedHidingRequest(FRecipeData RemovedRecipeData);
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UHorizontalBox* ActiveRequestWidgetsList;
private:
	void RemoveRequestFromActiveRequestWidgetList(FRecipeData& RemovedRecipeData);
};
