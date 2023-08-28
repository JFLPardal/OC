// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "DeliveryConveyorActor.h"
#include "Plate.h"
#include "Ingredient.h"

#if WITH_DEV_AUTOMATION_TESTS

EAutomationTestFlags::Type const defaultFlags = static_cast<EAutomationTestFlags::Type>(EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIfInteractingWithNullptrThenInteractionOutcomeShouldBeNoInteraction, "OC.Actors.DeliveryConveyorActor.IfInteractingWithNullptrThenInteractionOutcomeShouldBeNoInteraction", defaultFlags)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIfPlateClearedThenPlateShouldHaveNoIngredients, "OC.Actors.Plate.IfPlateClearedThenPlateShouldHaveNoIngredients", defaultFlags)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIfIngredientInteractsWithNewPlateThenPlateShouldHaveIngredients, "OC.Actors.Plate.IfIngredientInteractsWithNewPlateThenPlateShouldHaveIngredients", defaultFlags)

bool FIfInteractingWithNullptrThenInteractionOutcomeShouldBeNoInteraction::RunTest(const FString& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	ADeliveryConveyorActor* const DeliveryConveyorActor = World->SpawnActor<ADeliveryConveyorActor>();
	auto const InteractionResult = DeliveryConveyorActor->AttemptInteractionWith(nullptr);

	TestEqual("interaction with nullptr should return 'NoInteraction'", InteractionResult.Outcome, EInteractableInteractionOutcome::NoInteraction);

	return true;
}

bool FIfPlateClearedThenPlateShouldHaveNoIngredients::RunTest(const FString& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	APlate* const Plate = World->SpawnActor<APlate>();

	Plate->ClearPlate();
	bool const IsPlateEmpty = Plate->IsEmpty();

	TestTrue("plate should have 0 ingredients after being cleared", IsPlateEmpty);

	return true;
}

bool FIfIngredientInteractsWithNewPlateThenPlateShouldHaveIngredients::RunTest(const FString& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	APlate* const Plate = World->SpawnActor<APlate>();
	AIngredient* const Ingredient = World->SpawnActor<AIngredient>();
	Ingredient->SetIngredient(EIngredient::Onion);
	Plate->AttemptInteractionWith(Ingredient);


	bool const IsPlateEmpty = Plate->IsEmpty();

	TestTrue("after adding an ingredient to a new plate, plate should have ingredients", !IsPlateEmpty);

	return true;
}

#endif //WITH_DEV_AUTOMATION_TESTS