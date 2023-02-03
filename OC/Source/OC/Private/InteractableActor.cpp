// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/StaticMeshComponent.h"
#include "Macros.h"
#include "RequestsSubsystem.h"


// Sets default values
AInteractableActor::AInteractableActor()
	:InteractableType(EInteractableType::Unspecified)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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