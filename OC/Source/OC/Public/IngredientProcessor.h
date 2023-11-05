// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticInteractableWithSocket.h"
#include "IngredientProcessor.generated.h"

UCLASS()
class OC_API AIngredientProcessor : public AStaticInteractableWithSocket
{
	GENERATED_BODY()
	
public:
	AIngredientProcessor();
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
};
