// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "EIngredient.h"
#include "DeliveryConveyorActor.generated.h"

//UDELEGATE(BlueprintCallable)
//DECLARE_DELEGATE_OneParam(FOnPlateDelivered, FRecipes*, Recipe)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlateDelivered, EIngredient, Recipe); // test as some work will be needed to get the recipe from the plate

class APlate;
/**
 * 
 */
UCLASS()
class OC_API ADeliveryConveyorActor : public AInteractableActor
{
	GENERATED_BODY()
public:
	ADeliveryConveyorActor();
	
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
