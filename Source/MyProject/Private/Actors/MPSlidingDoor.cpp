// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MPSlidingDoor.h"
#include "TimerManager.h"

// Sets default values
AMPSlidingDoor::AMPSlidingDoor()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMPSlidingDoor::BeginPlay()
{
	Super::BeginPlay();

	// Check for static mesh
	if (DoorMesh)
	{
		ClosedPosition = DoorMesh->GetRelativeLocation();
		OpenPosition = ClosedPosition + OpenOffset;

		// Set initial position
		DoorMesh->SetRelativeLocation(bStartOpen ? OpenPosition : ClosedPosition);
	}
}

void AMPSlidingDoor::OnDoorOpened_Implementation()
{
	// Using Timeline in blueprint to handle movement
}

void AMPSlidingDoor::OnDoorClosed_Implementation()
{
	// Using Timeline in blueprint to handle movement
}

