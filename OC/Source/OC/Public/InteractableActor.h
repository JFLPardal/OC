// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EInteractableType.h"
#include "InteractableActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class OC_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	virtual void AttemptInteractionWith(AInteractableActor* otherInteractable);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Setup")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category="Interactable")
	EInteractableType InteractableType;
};
