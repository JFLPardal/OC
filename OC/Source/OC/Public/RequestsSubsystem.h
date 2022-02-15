// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RequestsSubsystem.generated.h"

class UDataTable;
struct FRecipes;
/**
 * Subsystem responsible for storing and all the logic related with Requests
 */
UCLASS()
class OC_API URequestsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	URequestsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
private:
	UPROPERTY(VIsibleAnywhere)
	UDataTable* RecipesDataTable;

	FRecipes* CurrentRecipeData;
	FString RecipesDataTableAssetLocation;
};
