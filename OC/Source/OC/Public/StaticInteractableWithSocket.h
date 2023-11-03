// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "StaticInteractableWithSocket.generated.h"

/**
 * 
 */
UCLASS()
class OC_API AStaticInteractableWithSocket : public AInteractableActor
{
	GENERATED_BODY()
public:
	AStaticInteractableWithSocket();

	virtual FInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable) override;
protected:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Socket")
	USceneComponent* Socket;

	AInteractableActor* InteractableInSocket;
};
