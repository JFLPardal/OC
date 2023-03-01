// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UOCUWLevelTimerHUD.generated.h"

class AOCGameModeBase;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class OC_API UUOCUWLevelTimerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetGameMode(AOCGameModeBase* GameMode);
protected:
	UFUNCTION()
	void OnLevelTimerUpdated(float TimeRemainingInLevelInSecs);

	virtual void BeginDestroy() override;
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* TimeLeftProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeRemainingInLevelText;

	AOCGameModeBase* GameMode;
private:
	void UpdateTimeRemainingInLevelText(float TimeRemainingInLevelInSecs);
	void UpdateTimeRemainingInLevelProgressBar(float TimeRemainingInLevelInSecs);
private:
	float InitialTimeRemainingInLevelInSecs;
};
