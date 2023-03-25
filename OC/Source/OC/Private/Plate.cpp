// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate.h"
#include "Ingredient.h"

APlate::APlate()
    : AInteractableActor()
    , CurrentRecipeData()
{
    InteractableType = EInteractableType::Plate;

    IngredientSocket = CreateDefaultSubobject<USceneComponent>("IngredientSocket");
    HeldIngredient = nullptr;
}

FInteractionOutcome APlate::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
    auto interactionOutcome = FInteractionOutcome(EInteractableInteractionOutcome::NoInteraction);
    const bool characterIsHoldingSomething = otherInteractable != nullptr;

    if(characterIsHoldingSomething)
    {
        if(otherInteractable->GetInteractableType() == EInteractableType::Ingredient)
        {
            const auto Ingredient = Cast<AIngredient>(otherInteractable)->GetIngredient();
            if(CurrentRecipeData.CanAddIngrendient(Ingredient))
            {
                CurrentRecipeData.AddIngredient(Ingredient);
                HeldIngredient = otherInteractable;
                OnPlateCompositionChanged.ExecuteIfBound(HeldIngredient);
                
                auto MeshComponent = HeldIngredient->FindComponentByClass<UStaticMeshComponent>();
                if(MeshComponent)
                {
                    FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
                    HeldIngredient->GetRootComponent()->AttachToComponent(IngredientSocket, attachmentRules);
                    MeshComponent->SetGenerateOverlapEvents(false);
                }
                
                interactionOutcome.Outcome = EInteractableInteractionOutcome::ShouldDetachFromCharacter;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Plate interaction with something with no interaction defined"));
        }
    }
    else
    {
        interactionOutcome.Outcome = EInteractableInteractionOutcome::ShouldAttachToCharacter;
    }

    return interactionOutcome;
}

const FRecipeData& APlate::GetRecipeData() const
{
    return CurrentRecipeData;
}

void APlate::ClearPlate()
{
    if(HeldIngredient)
    {
        HeldIngredient->Destroy();
        HeldIngredient = nullptr;
        OnPlateCompositionChanged.ExecuteIfBound(HeldIngredient);
    }
    CurrentRecipeData = FRecipeData{};
}