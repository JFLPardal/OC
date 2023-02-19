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

class UActiveRecipeWidget;
class URequestsSubsystem;

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreatedRequestWidget, UActiveRecipeWidget*, AddedWidget);

UCLASS()
class OC_API AOCGameModeBase : public AGameModeBase, public IVisualLoggerDebugSnapshotInterface
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FCreatedRequestWidget OnCreatedRequestWidget;

	// TODO move this somewhere appropriate
	UFUNCTION(BlueprintCallable, Category = "Recipes")
	bool AreRecipesTheSame( FRecipeData const& Recipe, FRecipeData const& OtherRecipe) const;

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
private:
	URequestsSubsystem* RequestsSubsystem;
	
	UPROPERTY(EditAnywhere, Category = "Requests")
	TSubclassOf<UActiveRecipeWidget> ActiveRecipeWidgetBlueprint;
	
	UPROPERTY(EditAnywhere, Category = "Requests")
	TSubclassOf<UUserWidget> ActiveRequestsHUDElementBlueprint;
private:
#if ENABLE_VISUAL_LOG
	virtual void GrabDebugSnapshot(FVisualLogEntry* Snapshot) const override;
#endif
};
