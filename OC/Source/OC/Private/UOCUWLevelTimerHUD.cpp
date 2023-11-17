// Fill out your copyright notice in the Description page of Project Settings.


#include "UOCUWLevelTimerHUD.h"

#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "OC/OCGameModeBase.h"

bool UUOCUWLevelTimerHUD::Initialize()
{
	bool const initialized = Super::Initialize();

	return initialized;
}

void UUOCUWLevelTimerHUD::SetGameMode(AOCGameModeBase* GameModeToSet)
{
	GameMode = GameModeToSet;
	InitialTimeRemainingInLevelInSecs = GameMode->GetInitialTimeRemainingInLevelInSecs();
	TimeLeftProgressBar->SetPercent(1.0f);
	UpdateTimeRemainingInLevelText(InitialTimeRemainingInLevelInSecs);
	UpdateTimeRemainingInLevelProgressBar(InitialTimeRemainingInLevelInSecs);

	GameMode->OnUpdatedTimeRemainingInLevel.AddDynamic(this, &UUOCUWLevelTimerHUD::OnLevelTimerUpdated);
}

void UUOCUWLevelTimerHUD::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	{ // update progress bar 
		float const CurrentPercentage = TimeLeftProgressBar->GetPercent();
		float const IntendedPercentage = TargetTimeRemainingInLevelAsPercentage;
		float const LerpSpeed = ProgressBarLerpSpeed;
		const float LerpValue = FMath::Lerp(CurrentPercentage, IntendedPercentage, LerpSpeed);

		TimeLeftProgressBar->SetPercent(LerpValue);
		bool const IsPlayingCriticalTimeRemainingAnimation = IsAnimationPlaying(CriticalTimeRemainingImageAnimation);
		if (IsPlayingCriticalTimeRemainingAnimation && LerpValue < .01f)
		{
			StopAnimation(CriticalTimeRemainingImageAnimation);
		}
	}
}

void UUOCUWLevelTimerHUD::OnLevelTimerUpdated(float TimeRemainingInLevelInSecs)
{
	// investigate the usage of FDateTime and FTimeSpan

	//int32 Seconds = static_cast<int32>(TimeRemainingInLevelInSecs);
	//FDateTime TimeRemainingInSecs{ 1,1,1,0,0,Seconds,0 };

	UpdateTimeRemainingInLevelText(TimeRemainingInLevelInSecs);
	UpdateTimeRemainingInLevelProgressBar(TimeRemainingInLevelInSecs);

	bool const isTimeCritical = IsTimeCritical(TimeRemainingInLevelInSecs);
	if (isTimeCritical)
	{
		UpdateAnimationsForIsTimeCritical();
	}
}
void UUOCUWLevelTimerHUD::UpdateTimeRemainingInLevelText(float TimeRemainingInLevelInSecs)
{
	bool const isTimeCritical = IsTimeCritical(TimeRemainingInLevelInSecs);
	if (isTimeCritical)
	{
		TimeRemainingInLevelText->SetColorAndOpacity(CriticalTimeRemainingTextColor);
	}

	int const TimeRemainingInLevelInSecsAsInt = static_cast<int>(FMath::Clamp(FMath::Floor(TimeRemainingInLevelInSecs), 0.0f, InitialTimeRemainingInLevelInSecs));
	TimeRemainingInLevelText->SetText(FText::AsNumber(TimeRemainingInLevelInSecsAsInt));
}

void UUOCUWLevelTimerHUD::UpdateTimeRemainingInLevelProgressBar(float TimeRemainingInLevelInSecs)
{
	TargetTimeRemainingInLevelAsPercentage = TimeRemainingInLevelInSecs / InitialTimeRemainingInLevelInSecs;
}

void UUOCUWLevelTimerHUD::UpdateAnimationsForIsTimeCritical()
{
	bool const IsPlayingCriticalTimeRemainingAnimation = IsAnimationPlaying(CriticalTimeRemainingImageAnimation);
	if (CriticalTimeRemainingImageAnimation && !IsPlayingCriticalTimeRemainingAnimation)
	{
		PlayAnimation(CriticalTimeRemainingImageAnimation, 0.0f, 0, EUMGSequencePlayMode::Type::Forward, 1, true);
	}
}

bool UUOCUWLevelTimerHUD::IsTimeCritical(float TimeRemainingInLevelInSecs) const
{
	return TimeRemainingInLevelInSecs <= CriticalTimeThreshold;
}

void UUOCUWLevelTimerHUD::BeginDestroy()
{
	Super::BeginDestroy();

	if (GameMode)
	{
		GameMode->OnUpdatedTimeRemainingInLevel.RemoveDynamic(this, &UUOCUWLevelTimerHUD::OnLevelTimerUpdated);
	}
}
