// Fill out your copyright notice in the Description page of Project Settings.


#include "UOCUWLevelTimerHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "OC\OCGameModeBase.h"

void UUOCUWLevelTimerHUD::SetGameMode(AOCGameModeBase* GameModeToSet)
{
	GameMode = GameModeToSet;
	InitialTimeRemainingInLevelInSecs = GameMode->GetInitialTimeRemainingInLevelInSecs();
	UpdateTimeRemainingInLevelText(InitialTimeRemainingInLevelInSecs);
	UpdateTimeRemainingInLevelProgressBar(InitialTimeRemainingInLevelInSecs);

	GameMode->OnUpdatedTimeRemainingInLevel.AddDynamic(this, &UUOCUWLevelTimerHUD::OnLevelTimerUpdated);
}

void UUOCUWLevelTimerHUD::OnLevelTimerUpdated(float TimeRemainingInLevelInSecs)
{
	// investigate the usage of FDateTime and FTimeSpan

	//int32 Seconds = static_cast<int32>(TimeRemainingInLevelInSecs);
	//FDateTime TimeRemainingInSecs{ 1,1,1,0,0,Seconds,0 };

	UpdateTimeRemainingInLevelText(TimeRemainingInLevelInSecs);
	UpdateTimeRemainingInLevelProgressBar(TimeRemainingInLevelInSecs);
}
void UUOCUWLevelTimerHUD::UpdateTimeRemainingInLevelText(float TimeRemainingInLevelInSecs)
{
	TimeRemainingInLevelText->SetText(FText::AsNumber(TimeRemainingInLevelInSecs));
}

void UUOCUWLevelTimerHUD::UpdateTimeRemainingInLevelProgressBar(float TimeRemainingInLevelInSecs)
{
	float const TimeLeftAsPercentage = TimeRemainingInLevelInSecs / InitialTimeRemainingInLevelInSecs;
	TimeLeftProgressBar->SetPercent(TimeLeftAsPercentage);
}

void UUOCUWLevelTimerHUD::BeginDestroy()
{
	Super::BeginDestroy();

	if (GameMode)
	{
		GameMode->OnUpdatedTimeRemainingInLevel.RemoveDynamic(this, &UUOCUWLevelTimerHUD::OnLevelTimerUpdated);
	}
}
