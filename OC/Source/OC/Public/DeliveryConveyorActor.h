// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticInteractableWithSocket.h"
#include "DeliveryConveyorActor.generated.h"

class APlate;

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlateDelivered, APlate*, Plate);

/**
 * 
 */
UCLASS()
class OC_API ADeliveryConveyorActor : public AStaticInteractableWithSocket
{
	GENERATED_BODY()
public:
	ADeliveryConveyorActor();
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FPlateDelivered OnPlateDelivered;
protected:
	UFUNCTION()
	void HideAndRespawnPlate();

	void InteractWithPlate(AInteractableActor* const otherInteractable);
private:
	void SetSpecificInteractionCallbacks();
protected:
	UPROPERTY(EditAnywhere, Category="Setup")
	AActor* PlateRespawnLocation;

	UPROPERTY(EditAnywhere, Category="Setup")
	float SecondsBeforePlateRespawn = 1.5f;
private:
	FTimerHandle hideAndRespawnPlate;
};
