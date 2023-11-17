// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Processable.generated.h"

UENUM(BlueprintType)
enum class EProcessableState : uint8
{
	NotProcessed 			UMETA(DisplayName = "NotProcessed"),
	PartiallyProcessed 		UMETA(DisplayName = "PartiallyProcessed"),
	FullyProcessed			UMETA(DisplayName = "FullyProcessed"),
	Default = NotProcessed,

	ReadyToAddToRecipe = FullyProcessed UMETA(DisplayName = "ReadyToAddToRecipe")
};
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UProcessable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OC_API IProcessable
{
	GENERATED_BODY()
public:
	virtual EProcessableState GetState() const = 0;
	virtual void Process() = 0;
	virtual bool IsReadyToUse() const = 0;
};
