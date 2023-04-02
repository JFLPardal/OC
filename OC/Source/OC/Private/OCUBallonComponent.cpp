// Fill out your copyright notice in the Description page of Project Settings.


#include "OCUBallonComponent.h"

#include "Plate.h"
#include "OCWBalloon.h"

void UOCUBallonComponent::BeginPlay()
{
	Super::BeginPlay();

	BalloonWidget = Cast<UOCWBalloon>(GetWidget());
	ensureMsgf(BalloonWidget, TEXT("Could not cast WidgetClass under 'User Interface' to UOCWBalloon for actor %s"), *GetOwner()->GetFName().ToString());

	APlate* Owner = Cast<APlate>(GetOwner());
	if (ensureMsgf(Owner, TEXT("Could not cast Owner to APlate for actor %s. UOCBalloonComponent can only be attached to APlate"), *GetOwner()->GetFName().ToString()))
	{
		Owner->OnPlateCompositionChanged.BindUObject(this, &UOCUBallonComponent::OnPlateCompositionChanged);
	}
}

void UOCUBallonComponent::OnPlateCompositionChanged(AInteractableActor const * const NewPlateComposition)
{
	if (BalloonWidget)
	{
		bool const ShouldHideBallon = NewPlateComposition == nullptr;
		if (ShouldHideBallon)
		{
			BalloonWidget->Hide();
		}
		else
		{
			BalloonWidget->Show();
			BalloonWidget->UpdateIngredientsWidget(NewPlateComposition);
		}
	}
}
