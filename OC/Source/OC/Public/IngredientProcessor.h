// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticInteractableWithSocket.h"
#include "Usable.h"
#include "IngredientProcessor.generated.h"

UCLASS()
class OC_API AIngredientProcessor : public AStaticInteractableWithSocket, public IUsable
{
	GENERATED_BODY()
	
public:
	AIngredientProcessor();

	virtual EUsageOutcome AttemptUse() override;
	virtual bool CanUse() const override;
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
};
