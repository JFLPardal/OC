// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "DeliveryConveyorActor.generated.h"


/**
 * 
 */
UCLASS()
class OC_API ADeliveryConveyorActor : public AInteractableActor
{
	GENERATED_BODY()
public:
	ADeliveryConveyorActor();
	
	// pure virtual from parent class
	EInteractableInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* PlateSocket;
};
