// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RecipeData.h"
#include "ActiveRecipeWidget.generated.h"

/**
 * 
 */
UCLASS()
class OC_API UActiveRecipeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetRecipeData(FRecipeData Recipe);
private:
	FRecipeData RecipeData;	
};
