// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AInteractableActor::AInteractableActor()
	:InteractableType(EInteractableType::Unspecified)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = BaseMesh;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if(mesh.Succeeded())
	{
		BaseMesh->SetStaticMesh(mesh.Object);
	}
}

EInteractableInteractionOutcome AInteractableActor::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
	checkfSlow(false, TEXT("%s is not overriding AInteractableActor::AttemptInteractionWith"), *GetActorLabel());
	UE_LOG(LogTemp, Error, TEXT("This function should not be called! override 'AttemptInteractionWith'"));
	return EInteractableInteractionOutcome::NoInteraction;
}

EInteractableType AInteractableActor::GetInteractableType() const
{
	return InteractableType;
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}