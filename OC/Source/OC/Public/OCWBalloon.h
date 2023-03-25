// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OCWBalloon.generated.h"

/**
 * 
 */

class UCanvasPanel;

UCLASS()
class OC_API UOCWBalloon : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	void Show();
	void Hide();
private:
	// there seems to be a bug with changing the visibility of a widget directly from code at runtime, this is a workaround
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* CanvasPanel = nullptr;
private:
	void SetCanvasVisibility(ESlateVisibility VisibilityToSet);
};
