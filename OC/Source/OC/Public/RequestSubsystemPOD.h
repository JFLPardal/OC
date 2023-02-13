// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RequestSubsystemPOD.generated.h"

/**
 * 
 */
UCLASS()
class OC_API URequestSubsystemPOD : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "RequestGenerationIntervals")
	float SecondsBeforeGeneratingFirstRecipe = .5f;

	UPROPERTY(EditAnywhere, Category = "RequestGenerationIntervals")
	float SecondsBeforeGeneratingSubsequentRequests = 3.0f;
	
};
