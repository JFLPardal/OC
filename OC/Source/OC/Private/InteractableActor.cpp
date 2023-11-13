// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/StaticMeshComponent.h"

using InteractableTypesAttachmentMap = std::unordered_map<EInteractableType, std::unordered_set<EInteractableType>>;
InteractableTypesAttachmentMap AInteractableActor::s_InteractableTypesAttachmentMap
{
	{EInteractableType::Ingredient, std::unordered_set<EInteractableType>{EInteractableType::Plate}}
};

AInteractableActor::AInteractableActor()
	: InteractableType(EInteractableType::Unspecified)
	, InteractableState(EInteractableState::Enabled)
{
	PrimaryActorTick.bCanEverTick = false;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = BaseMesh;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if(mesh.Succeeded())
	{
		BaseMesh->SetStaticMesh(mesh.Object);
	}
}

FInteractionOutcome AInteractableActor::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
	checkfSlow(false, TEXT("%s is not overriding AInteractableActor::AttemptInteractionWith"), *GetActorLabel());
	UE_LOG(LogTemp, Error, TEXT("This function should not be called! override 'AttemptInteractionWith' for actor %s"), *GetActorLabel());
	return FInteractionOutcome(EInteractableInteractionOutcome::NoInteraction);
}

EInteractableType AInteractableActor::GetInteractableType() const
{
	return InteractableType;
}

void AInteractableActor::DisableInteraction()
{
	InteractableState = EInteractableState::Disabled;
}

void AInteractableActor::EnableInteraction()
{
	InteractableState = EInteractableState::Enabled;
}

bool AInteractableActor::IsInteractionEnabled() const
{
	return InteractableState == EInteractableState::Enabled;
}

bool AInteractableActor::CanAttachToInteractable(EInteractableType interactableToBeAttached, EInteractableType interactableToAttachTo)
{
	bool canAttach = false;

	auto interactableToBeAttachedPossibleAttachmentsIt = s_InteractableTypesAttachmentMap.find(interactableToBeAttached);
	if (interactableToBeAttachedPossibleAttachmentsIt != s_InteractableTypesAttachmentMap.cend())
	{
		auto interactableToBeAttachedPossibleAttachments = interactableToBeAttachedPossibleAttachmentsIt->second;
		auto interactableToAttachToIt = interactableToBeAttachedPossibleAttachments.find(interactableToAttachTo);

		canAttach = interactableToAttachToIt != interactableToBeAttachedPossibleAttachments.cend();
	}

	return canAttach;
}


void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FInteractionOutcome::FInteractionOutcome()
	:FInteractionOutcome(EInteractableInteractionOutcome::NoInteraction)
{
}

FInteractionOutcome::FInteractionOutcome(EInteractableInteractionOutcome outcome, AInteractableActor* newActorToInteractWith)
{
	Outcome = outcome;
	NewActorToInteractWith = newActorToInteractWith;
}