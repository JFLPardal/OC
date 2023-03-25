// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "OCUBallonComponent.generated.h"

class AInteractableActor;
class UOCWBalloon;

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OC_API UOCUBallonComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
private:
	UOCWBalloon* BalloonWidget = nullptr;
private:
	void OnPlateCompositionChanged(AInteractableActor const* const NewPlateComposition);
};
