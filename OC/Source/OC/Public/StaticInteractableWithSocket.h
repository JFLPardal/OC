// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Containers/Map.h"
#include "Templates/Function.h"
#include "StaticInteractableWithSocket.generated.h"

UENUM(BlueprintType)
enum class EInteractableInteractionOptions : uint8
{
	CanOnlyAttach		UMETA(DisplayName = "CanOnlyAttach"),
	CanOnlyDetach		UMETA(DisplayName = "CanOnlyDetach"),
	CanAttachAndDetach	UMETA(DisplayName = "CanAttachAndDetach"),

	Default = CanAttachAndDetach
};
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
	bool CanAttachInteractable() const;
	bool CanDetachInteractable() const;

	bool HasInteractableInSocket() const;
private:
	bool CheckIfInteractableShouldAttachToThisAndDetachFromPlayer(FInteractionOutcome& interactionOutcome, AInteractableActor* const otherInteractable);
	bool CheckIfInteractableShouldDetachFromThisAndAttachToPlayer(FInteractionOutcome& interactionOutcome, AInteractableActor* const otherInteractable);
	bool CheckIfInteractableShouldAttachToInteractableInSocketAndDetachFromPlayer(FInteractionOutcome& interactionOutcome, AInteractableActor* const otherInteractable);
protected:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Socket")
	USceneComponent* Socket;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	EInteractableInteractionOptions InteractionOptions;

	AInteractableActor* InteractableInSocket;

	using InteractableTypeToInteractionExecution = TMap<EInteractableType, TFunction<void(AInteractableActor* const)>>;
	InteractableTypeToInteractionExecution InteractableTypeToInteraction;
};
