// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UOCUWLevelTimerHUD.generated.h"

class AOCGameModeBase;
class UProgressBar;
class UTextBlock;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class OC_API UUOCUWLevelTimerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	void SetGameMode(AOCGameModeBase* GameMode);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	UFUNCTION()
	void OnLevelTimerUpdated(float TimeRemainingInLevelInSecs);

	virtual void BeginDestroy() override;
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* TimeLeftProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeRemainingInLevelText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CriticalTimeRemainingImageAnimation;


	AOCGameModeBase* GameMode;

	UPROPERTY(EditAnywhere, Category = "TimeRemainingText")
	float CriticalTimeThreshold = 10.0f;
	
	UPROPERTY(EditAnywhere)
	float ProgressBarLerpSpeed = .2f;
private:
	void UpdateTimeRemainingInLevelText(float TimeRemainingInLevelInSecs);
	void UpdateTimeRemainingInLevelProgressBar(float TimeRemainingInLevelInSecs);
	void UpdateAnimationsForIsTimeCritical();

	bool IsTimeCritical(float TimeRemainingInLevelInSecs) const;
private:
	float InitialTimeRemainingInLevelInSecs;
	float TargetTimeRemainingInLevelAsPercentage = 1.0f;
};
