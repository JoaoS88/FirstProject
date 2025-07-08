// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MPDoorBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AMPDoorBase::AMPDoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and setup components
	// Frame
	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	RootComponent = FrameMesh;
	// Door
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMPDoorBase::BeginPlay()
{
	Super::BeginPlay();

	// Initialise the door
	InitializeDoor();

}

void AMPDoorBase::OnDoorOpened_Implementation()
{
	// In blueprint
}

void AMPDoorBase::OnDoorClosed_Implementation()
{
	// In blueprint
}

void AMPDoorBase::OnInteract_Implementation(AActor* Interactor)
{
	ToggleDoor(Interactor);
}

FText AMPDoorBase::GetInteractText_Implementation() const
{
	// Interaction text prompt
	return FText::FromString(bIsOpen ? "Close Door" : "Open Door");
}

bool AMPDoorBase::CanInteract_Implementation() const
{
	// To be overriden in child classes (e.g. locked doors)
	return true;
}

void AMPDoorBase::ToggleDoor(AActor* Interactor)
{
	bIsOpen = !bIsOpen;

	if (bIsOpen)
	{
		OnDoorOpened();
	}
	else
	{
		OnDoorClosed();
	}

	PlayDoorEffects(Interactor);
}

void AMPDoorBase::PlayDoorEffects_Implementation(AActor* Interactor)
{
	UE_LOG(LogTemp, Log, TEXT("%s: Door toggled by %s"), *GetName(), *GetNameSafe(Interactor));

	if (DoorSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
	}
}

void AMPDoorBase::InitializeDoor()
{
	// make sure visual state matches logical state
	bIsOpen = bStartOpen;

	if (bIsOpen)
	{
		OnDoorOpened(); 
	}
	else
	{
		OnDoorClosed(); 
	}
}
