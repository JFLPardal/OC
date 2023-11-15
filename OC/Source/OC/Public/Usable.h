// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Usable.generated.h"


UENUM(BlueprintType)
enum class EUsageOutcome : uint8
{
	FailedToUse			UMETA(DisplayName = "FailedToUse"),
	FullyProcessed 		UMETA(DisplayName = "FullyProcessed"),
	NotFullyProcessed 	UMETA(DisplayName = "NotFullyProcessed"),
	Default = FailedToUse
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUsable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OC_API IUsable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanUse() const = 0;
	virtual EUsageOutcome TryToUse() = 0;

};
