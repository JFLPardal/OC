// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/StaticMeshComponent.h"
#include "RequestsSubsystem.h"


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