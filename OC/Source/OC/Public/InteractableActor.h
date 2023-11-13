// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EInteractableType.h"
#include <unordered_set>
#include <unordered_map>

#include "InteractableActor.generated.h"

class UStaticMeshComponent;

/**
 *
 */
UENUM(BlueprintType)
enum class EInteractableInteractionOutcome : uint8
{
	NoInteraction 						UMETA(DisplayName="NoInteraction"),
	ShouldDetachFromCharacter 			UMETA(DisplayName="ShouldDetachFromCharacter"),
	ShouldAttachToCharacter 			UMETA(DisplayName="ShouldAttachToCharacter"),
	InteractWithOtherInteractable		UMETA(DisplayName="InteractWithOtherInteractable"),
	InteractWithInteractableInSocket	UMETA(DisplayName="InteractWithInteractableInSocket"),
};

enum class EInteractableState
{
	Enabled,
	Disabled
};

USTRUCT(BlueprintType)
struct FInteractionOutcome
{
	GENERATED_BODY() 

	FInteractionOutcome();
	explicit FInteractionOutcome(EInteractableInteractionOutcome outcome, AInteractableActor* newActorToInteractWith = nullptr);


	UPROPERTY(BlueprintReadWrite)
	EInteractableInteractionOutcome Outcome;
	UPROPERTY(BlueprintReadWrite)
	AInteractableActor* NewActorToInteractWith;
};

UCLASS()
class OC_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	virtual FInteractionOutcome AttemptInteractionWith(AInteractableActor* otherInteractable);

	EInteractableType GetInteractableType() const;
	
	void DisableInteraction();
	void EnableInteraction();
	bool IsInteractionEnabled() const;

	static bool CanAttachToInteractable(EInteractableType interactableToBeAttached, EInteractableType interactableToAttachTo);
protected:
	virtual void BeginPlay() override;
protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Setup")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category="Interactable")
	EInteractableType InteractableType;

	EInteractableState InteractableState;
private:
	using InteractableTypesAttachmentMap = std::unordered_map<EInteractableType, std::unordered_set<EInteractableType>>;
	static InteractableTypesAttachmentMap s_InteractableTypesAttachmentMap;
};
