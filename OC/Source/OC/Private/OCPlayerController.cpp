// Fill out your copyright notice in the Description page of Project Settings.


#include "OCPlayerController.h"

#include "InteractableActor.h"
#include "Components\InputComponent.h"
#include "Components\SphereComponent.h"
#include "Kismet\GameplayStatics.h"
#include "GameFramework\Character.h"

void AOCPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (ensureMsgf(InputComponent, TEXT("Could not find InputComponent in Player Actor")))
    {
        InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AOCPlayerController::TryToInteract);
    }
    
    AActor* PlayerCharacter = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacter::StaticClass());
    if (ensureMsgf(PlayerCharacter, TEXT("Could not find TriggerVolume in Player Actor")))
    {
        TriggerVolume = PlayerCharacter->FindComponentByClass<USphereComponent>();
        ensureMsgf(TriggerVolume, TEXT("Could not find TriggerVolume in Player Actor"));

        // TODO this is not a good way to do this as a lot of different components can be scene components on the actor.
        // Potentially create a SocketComponent, make InteractableSocket on BP_ControlableActor use that as its base class and update this
        InteractableSocket = Cast<USceneComponent>(PlayerCharacter->GetDefaultSubobjectByName(TEXT("InteractableSocket")));
        ensureMsgf(InteractableSocket, TEXT("Could not find USceneComponent named \"InteractableSocket\" in Player Actor"));
    }
}

void AOCPlayerController::TryToInteract()
{
    bool const IsHoldingInteractable = AttachedInteractable != nullptr;
    if (IsHoldingInteractable)
    {
        if (AInteractableActor* OverlappingInteractable = IsAnotherInteractableInRadius())
        {
            FInteractionOutcome InteractionOutcome = OverlappingInteractable->AttemptInteractionWith(AttachedInteractable);
            EInteractableInteractionOutcome Outcome = InteractionOutcome.Outcome;

            switch (Outcome)
            {
            case EInteractableInteractionOutcome::ShouldDetachFromCharacter:
                ResetAttachedInteractable();
                break;
            case EInteractableInteractionOutcome::InteractWithOtherInteractable:
                OverlappingInteractable->AttemptInteractionWith(AttachedInteractable);
                break;
            case EInteractableInteractionOutcome::NoInteraction:
            case EInteractableInteractionOutcome::ShouldAttachToCharacter:
                break;
            default:
                UE_LOG(LogTemp, Error, TEXT("No interaction outcome defined for type %s"), Outcome);
                break;
            }
        }
        else
        {
            DropInteractable();
        }
    }
    else
    {
        if (AInteractableActor* OverlappingInteractable = IsAnotherInteractableInRadius())
        {
            FInteractionOutcome InteractionOutcome = OverlappingInteractable->AttemptInteractionWith(nullptr);
            EInteractableInteractionOutcome Outcome = InteractionOutcome.Outcome;

            switch (Outcome)
            {
            case EInteractableInteractionOutcome::ShouldDetachFromCharacter:
                ResetAttachedInteractable();
                break;
            case EInteractableInteractionOutcome::InteractWithOtherInteractable:
            {

                FInteractionOutcome InteractionOutcomeWithNewInteractable = InteractionOutcome.NewActorToInteractWith->AttemptInteractionWith(nullptr);
                EInteractableInteractionOutcome OutcomeWithNewInteractable = InteractionOutcomeWithNewInteractable.Outcome;

                switch (OutcomeWithNewInteractable)
                {

                case EInteractableInteractionOutcome::ShouldDetachFromCharacter:
                    ResetAttachedInteractable();
                    break;
                case EInteractableInteractionOutcome::ShouldAttachToCharacter:
                {
                    if (InteractableSocket)
                    {
                        AttachedInteractable = InteractionOutcome.NewActorToInteractWith;

                        EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
                        EAttachmentRule RotationRule = EAttachmentRule::KeepRelative;
                        EAttachmentRule ScaleRule = EAttachmentRule::KeepWorld;
                        bool const WieldSimulatedBodies = false;
                        FAttachmentTransformRules const AttachmentRules{ LocationRule, RotationRule, ScaleRule, WieldSimulatedBodies };

                        AttachedInteractable->AttachToComponent(InteractableSocket, AttachmentRules);
                        break;
                    }
                }
                case EInteractableInteractionOutcome::NoInteraction:
                case EInteractableInteractionOutcome::InteractWithOtherInteractable:
                    break;
                default:
                    UE_LOG(LogTemp, Error, TEXT("No interaction outcome defined for type %s"), Outcome);
                    break;
                }
                break;
            }
            case EInteractableInteractionOutcome::ShouldAttachToCharacter:
            {
                if (InteractableSocket)
                {
                    AttachedInteractable = OverlappingInteractable;

                    EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
                    EAttachmentRule RotationRule = EAttachmentRule::KeepRelative;
                    EAttachmentRule ScaleRule = EAttachmentRule::KeepWorld;
                    bool const WieldSimulatedBodies = false;
                    FAttachmentTransformRules const AttachmentRules{ LocationRule, RotationRule, ScaleRule, WieldSimulatedBodies };

                    AttachedInteractable->AttachToComponent(InteractableSocket, AttachmentRules);
                    break;
                }
            }
            case EInteractableInteractionOutcome::NoInteraction:
                break;
            default:
                UE_LOG(LogTemp, Error, TEXT("No interaction outcome defined for type %s"), Outcome);
                break;
            }
        }
    }
}

AInteractableActor* AOCPlayerController::IsAnotherInteractableInRadius()
{
    AInteractableActor* OverlappingInteractable = nullptr;
    
    if (TriggerVolume)
    {
        TArray<AActor*> OverlappingActors;
        TriggerVolume->GetOverlappingActors(OverlappingActors, AInteractableActor::StaticClass());

        bool const IsHoldingInteractable = AttachedInteractable != nullptr;
        // > 1 because the attached actor is at least overlapping with the player
        int const indexToCheck = IsHoldingInteractable ? 1 : 0;
        bool const IsAttachedInteractableOverlappingWithOtherActor = OverlappingActors.Num() > indexToCheck;
        if (IsAttachedInteractableOverlappingWithOtherActor)
        {
            OverlappingInteractable = Cast<AInteractableActor>(OverlappingActors[indexToCheck]);
        }
    }

    return OverlappingInteractable;
}

void AOCPlayerController::DropInteractable()
{
    if (AttachedInteractable)
    {
        if (USceneComponent* RootComponentForAttachedInteractable = AttachedInteractable->GetRootComponent())
        {
            bool const bCallModify = true;
            FDetachmentTransformRules DetachmentRules{ EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, EDetachmentRule::KeepWorld, bCallModify };

            RootComponentForAttachedInteractable->DetachFromComponent(DetachmentRules);
        }
    }

    ResetAttachedInteractable();
}

void AOCPlayerController::ResetAttachedInteractable()
{
    AttachedInteractable = nullptr;
}
