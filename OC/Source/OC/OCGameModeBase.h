// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VisualLogger/VisualLoggerDebugSnapshotInterface.h"
#include "RecipeData.h"

#include "OCGameModeBase.generated.h"

/**
 * 
 */

class UActiveRecipeWidgetUOCUWLevelTimerHUD;
class UUOCUWLevelTimerHUD;
class URequestsSubsystem;

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreatedRequestWidget, UActiveRecipeWidget*, AddedWidget);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdatedTimeRemainingInLevel, float, TimeRemainingInLevelInSecs);

UCLASS()
class OC_API AOCGameModeBase : public AGameModeBase, public IVisualLoggerDebugSnapshotInterface
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	float GetInitialTimeRemainingInLevelInSecs() const { return TimeRemainingInLevelInSecs; }

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FCreatedRequestWidget OnCreatedRequestWidget;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FUpdatedTimeRemainingInLevel OnUpdatedTimeRemainingInLevel;

	UFUNCTION()
	void GeneratedNewRequest(const FRecipeData& GeneratedRequestData);
	UFUNCTION()
	void CompletedRequest(FRecipeData CompletedRequestData);
	
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetActiveRequestsHUDElement() const { return ActiveRequestsHUDElement; }

	UFUNCTION(Exec, Category = "Requests")
	void DebugGenerateNewRequest();

	UFUNCTION(Exec, Category = "Requests")
	void DebugCompleteOldestRequest();
protected:
	TArray<UActiveRecipeWidget*> ActiveRequestWidgetsArray;

	UPROPERTY(BlueprintReadWrite, Category = "Requests")
	UUserWidget* ActiveRequestsHUDElement;

	UPROPERTY(BlueprintReadWrite, Category = "LevelOverConditions")
	UUOCUWLevelTimerHUD* LevelTimerWidget;
	
	UPROPERTY(EditAnywhere, Category = "LevelOverConditions")
	float SecondsBetweenTimerUpdate = 1.0f;
private:
	URequestsSubsystem* RequestsSubsystem;
	
	UPROPERTY(EditAnywhere, Category = "Requests")
	TSubclassOf<UActiveRecipeWidget> ActiveRecipeWidgetBlueprint;
	
	UPROPERTY(EditAnywhere, Category = "Requests")
	TSubclassOf<UUserWidget> ActiveRequestsHUDElementBlueprint;

	UPROPERTY(EditAnywhere, Category="LevelOverConditions")
	TSubclassOf<UUOCUWLevelTimerHUD> LevelTimerWidgetBlueprint;

	UPROPERTY(EditAnywhere, Category = "LevelOverConditions")
	bool bShouldTimerExpiredFinishGame = false;
	UPROPERTY(EditAnywhere, Category = "LevelOverConditions")
	float TimeRemainingInLevelInSecs = 10.f;
	FTimerHandle TimerToFinishLevel;
	FTimerManager* TimerManager;
private:
	UFUNCTION()
	void DecreaseTimerRemainingInLevel();
	void FinishLevel();
	void SetLevelTimer();

#if ENABLE_VISUAL_LOG
	virtual void GrabDebugSnapshot(FVisualLogEntry* Snapshot) const override;
#endif
};
