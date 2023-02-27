// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UOCUWLevelTimerHUD.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class OC_API UUOCUWLevelTimerHUD : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* TimeLeftProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock * TimeLeftText;
};
