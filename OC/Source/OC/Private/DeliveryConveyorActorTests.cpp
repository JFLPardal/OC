// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "DeliveryConveyorActor.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIfInteractingWithNullptrThenInteractionOutcomeShouldBeNoInteraction,"OC.Actors.DeliveryConveyorActor.IfInteractingWithNullptrThenInteractionOutcomeShouldBeNoInteraction", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FIfInteractingWithNullptrThenInteractionOutcomeShouldBeNoInteraction::RunTest(const FString& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	ADeliveryConveyorActor* const DeliveryConveyorActor = World->SpawnActor<ADeliveryConveyorActor>();
	auto const InteractionResult = DeliveryConveyorActor->AttemptInteractionWith(nullptr);
	
	TestEqual("interaction with nullptr should return 'NoInteraction'", InteractionResult.Outcome, EInteractableInteractionOutcome::NoInteraction);

	return true;
}

#endif //WITH_DEV_AUTOMATION_TESTS