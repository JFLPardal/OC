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

void AInteractableActor::AttemptInteractionWith(AInteractableActor* otherInteractable)
{
	if(otherInteractable)
	{
		if(otherInteractable->InteractableType == EInteractableType::Plate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Interacting with Plate"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Interacting with something for which interaction is unspecified"));
		}
	}
}
