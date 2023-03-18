// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OCPlayerController.generated.h"

class AInteractableActor;
class USphereComponent;
class USceneComponent;

/**
 * 
 */
UCLASS()
class OC_API AOCPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;
private:
	void TryToInteract();
	AInteractableActor* IsAnotherInteractableInRadius();
	void AttachInteractable(AInteractableActor* ActorToAttach);
	void DropInteractable();
	void ResetAttachedInteractable();
private:
	AInteractableActor* AttachedInteractable = nullptr;
	USphereComponent* TriggerVolume = nullptr;
	USceneComponent* InteractableSocket = nullptr;
};
