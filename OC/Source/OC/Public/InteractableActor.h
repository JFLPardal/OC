// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EInteractableType.h"
#include "InteractableActor.generated.h"

class UStaticMeshComponent;

/**
 *
 */
UENUM(BlueprintType)
enum class EInteractableInteractionOutcome : uint8
{
	NoInteraction 				UMETA(DisplayName="NoInteraction"),
	ShouldDetachFromCharacter 	UMETA(DisplayName="ShouldDetachFromCharacter"),
	ShouldAttachToCharacter 	UMETA(DisplayName="ShouldAttachToCharacter"),
};

UCLASS()
class OC_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual EInteractableInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable);

	EInteractableType GetInteractableType() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Setup")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category="Interactable")
	EInteractableType InteractableType;
};
