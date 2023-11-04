// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate.h"
#include "Ingredient.h"

APlate::APlate()
    : AInteractableActor()
    , CurrentRecipeData()
{
    InteractableType = EInteractableType::Plate;
}

void APlate::BeginPlay()
{
    Super::BeginPlay();

    ensureMsgf(!IngredientSocketName.IsNone(), TEXT("no ingredient socket name defined for plate"));
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
                HeldIngredients.Add(otherInteractable);
                if (AInteractableActor* AddedIngredient = HeldIngredients.Last())
                {
                    OnPlateCompositionChanged.ExecuteIfBound(AddedIngredient);

                    if(auto MeshComponent = AddedIngredient->FindComponentByClass<UStaticMeshComponent>())
                    {
                        if (auto PlateMesh = FindComponentByClass<UStaticMeshComponent>(); PlateMesh && !IngredientSocketName.IsNone())
                        {
                            FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
                            AddedIngredient->GetRootComponent()->AttachToComponent(PlateMesh, attachmentRules, IngredientSocketName);
                            AddedIngredient->DisableInteraction();
                        }
                        MeshComponent->SetGenerateOverlapEvents(false);
                    }
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

bool APlate::IsEmpty() const
{
    return HeldIngredients.IsEmpty();
}

void APlate::ClearPlate()
{
    for(auto& HeldIngredient : HeldIngredients)
    {
        HeldIngredient->Destroy();
        HeldIngredient = nullptr;
        OnPlateCompositionChanged.ExecuteIfBound(HeldIngredient);
    }
    HeldIngredients.Empty();
    CurrentRecipeData = FRecipeData{};

    EnableInteraction();
}