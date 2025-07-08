// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MPButton.h"

// Sets default values
AMPButton::AMPButton()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMPButton::BeginPlay()
{
	Super::BeginPlay();

	// Check for static mesh
	if (BaseMesh)
	{
		// Get initial location of mesh
		InitialLocation = BaseMesh->GetRelativeLocation();
	}
}

void AMPButton::PlayActivationEffects_Implementation(AActor* Interactor)
{
	// Call parent class's effects
	Super::PlayActivationEffects_Implementation(Interactor);

	// Check for static mesh
	if (BaseMesh)
	{
		// Offset in Z based on if button is pressed down or not
		const float Offset = bIsActive ? PressDepth : 0.0f;
		FVector NewLocation = InitialLocation + FVector(0, 0, Offset);
		BaseMesh->SetRelativeLocation(NewLocation);
	}
}

FText AMPButton::GetInteractText_Implementation() const
{
	return FText::FromString(bIsActive ? TEXT("Release Button") : TEXT("Press Button"));
}