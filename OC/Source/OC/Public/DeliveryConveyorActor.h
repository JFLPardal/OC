// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "DeliveryConveyorActor.generated.h"

class APlate;

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlateDelivered, APlate*, Plate);

/**
 * 
 */
UCLASS()
class OC_API ADeliveryConveyorActor : public AInteractableActor
{
	GENERATED_BODY()
public:
	ADeliveryConveyorActor();
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FPlateDelivered OnPlateDelivered;
	
	// pure virtual from parent class
	FInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
protected:
	UFUNCTION()
	void HideAndRespawnPlate();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* PlateSocket;

	UPROPERTY(EditAnywhere, Category="Setup")
	AActor* PlateRespawnLocation;

	UPROPERTY(EditAnywhere, Category="Setup")
	float SecondsBeforePlateRespawn = 1.5f;
private:
	FTimerHandle hideAndRespawnPlate;
	APlate* HeldPlate;
};
