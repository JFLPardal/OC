// Fill out your copyright notice in the Description page of Project Settings.


#include "OCPlayerController.h"

#include "InteractableActor.h"
#include "Components\InputComponent.h"
#include "Components\SphereComponent.h"
#include "Kismet\GameplayStatics.h"
#include "GameFramework\Character.h"
#include "Usable.h"

FString InteractableSocketIsNullptr = "Could not find USceneComponent named \"InteractableSocket\" in Player Actor";

void AOCPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (ensureMsgf(InputComponent, TEXT("Could not find InputComponent in Player Actor")))
    {
        InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AOCPlayerController::TryToInteract);
        InputComponent->BindAction("Use", EInputEvent::IE_Pressed, this, &AOCPlayerController::TryToUse);
    }
    
    AActor* PlayerCharacter = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacter::StaticClass());
    if (ensureMsgf(PlayerCharacter, TEXT("Could not find TriggerVolume in Player Actor")))
    {
        TriggerVolume = PlayerCharacter->FindComponentByClass<USphereComponent>();
        ensureMsgf(TriggerVolume, TEXT("Could not find TriggerVolume in Player Actor"));

        // TODO this is not a good way to do this as a lot of different components can be scene components on the actor.
        // Potentially create a SocketComponent, make InteractableSocket on BP_ControlableActor use that as its base class and update this
        InteractableSocket = Cast<USceneComponent>(PlayerCharacter->GetDefaultSubobjectByName(TEXT("InteractableSocket")));
        ensureMsgf(InteractableSocket, TEXT("%s"), *InteractableSocketIsNullptr);
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
            case EInteractableInteractionOutcome::InteractWithInteractableInSocket:
            {
                FInteractionOutcome InteractionOutcomeWithInteractableInSocket = InteractionOutcome.NewActorToInteractWith->AttemptInteractionWith(AttachedInteractable);
                if (InteractionOutcomeWithInteractableInSocket.Outcome == EInteractableInteractionOutcome::ShouldDetachFromCharacter)
                {
                    ResetAttachedInteractable();
                }
                break;
            }
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
                    AttachInteractable(InteractionOutcome.NewActorToInteractWith);
                    break;
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
                AttachInteractable(OverlappingInteractable);
                break;
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

void AOCPlayerController::TryToUse()
{
    bool const IsHoldingInteractable = AttachedInteractable != nullptr;
    if (!IsHoldingInteractable)
    {
        if (IUsable* UsableInRadius = IsUsableInRadius())
        {
            EUsageOutcome UsageOutcome = UsableInRadius->TryToUse();

            switch (UsageOutcome)
            {
            case EUsageOutcome::FullyProcessed:
                UE_LOG(LogTemp, Error, TEXT("Fully processed"));
                break;
            case EUsageOutcome::NotFullyProcessed:
                UE_LOG(LogTemp, Error, TEXT("Not fully processed"));
                break;
            case EUsageOutcome::FailedToUse:
                UE_LOG(LogTemp, Error, TEXT("Failed to use"));
                break;
            default:
                UE_LOG(LogTemp, Error, TEXT("No usage outcome defined for type %s"), UsageOutcome);
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

        for (AActor*const OverlappingActor : OverlappingActors)
        {
            if (AInteractableActor* TempOverlappingInteractable = Cast<AInteractableActor>(OverlappingActor); TempOverlappingInteractable->IsInteractionEnabled())
            {
                OverlappingInteractable = TempOverlappingInteractable;
                break;
            }
        }
    }

    return OverlappingInteractable;
}

IUsable* AOCPlayerController::IsUsableInRadius()
{
    IUsable* UsableInRadius = nullptr;

    if (TriggerVolume)
    {
        TArray<AActor*> OverlappingActors;
        TriggerVolume->GetOverlappingActors(OverlappingActors, AInteractableActor::StaticClass());

        for (AActor* const OverlappingActor : OverlappingActors)
        {
            if (IUsable* TempOverlappingUsable = Cast<IUsable>(OverlappingActor))
            {
                UsableInRadius = TempOverlappingUsable;
                break;
            }
        }
    }

    return UsableInRadius;
}

void AOCPlayerController::AttachInteractable(AInteractableActor* ActorToAttach)
{
    
    if (ensureMsgf(InteractableSocket, TEXT("%s"), *InteractableSocketIsNullptr) && 
        ensureMsgf(ActorToAttach, TEXT("Trying to attach a nullptr")))
    {
        AttachedInteractable = ActorToAttach;

        EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
        EAttachmentRule RotationRule = EAttachmentRule::KeepRelative;
        EAttachmentRule ScaleRule = EAttachmentRule::KeepWorld;
        bool const WieldSimulatedBodies = false;
        FAttachmentTransformRules const AttachmentRules{ LocationRule, RotationRule, ScaleRule, WieldSimulatedBodies };

        AttachedInteractable->AttachToComponent(InteractableSocket, AttachmentRules);

        ActorToAttach->DisableInteraction();
    }
}

void AOCPlayerController::DropInteractable()
{
    if (AttachedInteractable)
    {
        if (USceneComponent* RootComponentForAttachedInteractable = AttachedInteractable->GetRootComponent())
        {
            EDetachmentRule LocationRule = EDetachmentRule::KeepWorld;
            EDetachmentRule RotationRule = EDetachmentRule::KeepWorld;
            EDetachmentRule ScaleRule = EDetachmentRule::KeepWorld;
            bool const bCallModify = true;

            FDetachmentTransformRules DetachmentRules
            (
                LocationRule,
                RotationRule,
                ScaleRule,
                bCallModify 
            );

            RootComponentForAttachedInteractable->DetachFromComponent(DetachmentRules);
            AttachedInteractable->EnableInteraction();
        }
    }

    ResetAttachedInteractable();
}

void AOCPlayerController::ResetAttachedInteractable()
{
    AttachedInteractable = nullptr;
}
