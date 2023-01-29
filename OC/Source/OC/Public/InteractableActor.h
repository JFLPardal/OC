// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EInteractableType.h"
#include "RecipeData.h"

#include "InteractableActor.generated.h"

class UStaticMeshComponent;

/**
 *
 */
UENUM(BlueprintType)
enum class EInteractableInteractionOutcome : uint8
{
	NoInteraction 					UMETA(DisplayName="NoInteraction"),
	ShouldDetachFromCharacter 		UMETA(DisplayName="ShouldDetachFromCharacter"),
	ShouldAttachToCharacter 		UMETA(DisplayName="ShouldAttachToCharacter"),
	InteractWithOtherInteractable	UMETA(DisplayName="InteractWithOtherInteractable"),
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

	UFUNCTION()
	void Fad(const FRecipeData& recipe);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Setup")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category="Interactable")
	EInteractableType InteractableType;
};
