// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UGrabbableComponent.h"

// Sets default values for this component's properties
UUGrabbableComponent::UUGrabbableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UUGrabbableComponent::BeginPlay()
{
	Super::BeginPlay();

	RootPrimitiveComponent = GetRootPrimitive();

	// Ensure simulates physics
	if (RootPrimitiveComponent && !RootPrimitiveComponent->IsSimulatingPhysics())
	{
		RootPrimitiveComponent->SetSimulatePhysics(true);
	}
}

// Functionality
void UUGrabbableComponent::OnGrab(AActor* Grabber)
{
	ActorGrabbing = Grabber;
	
	// Debug
	UE_LOG(LogTemp, Log, TEXT("%s grabbed by %s"), *GetOwner()->GetName(), *Grabber->GetName());
}
void UUGrabbableComponent::OnRelease(bool bFreeze)
{
	if (!RootPrimitiveComponent) return;

	// If bFreeze true then want to stop simulating physics
	if (bFreeze)
	{
		RootPrimitiveComponent->SetSimulatePhysics(false);
	}

	// No longer grabbing
	ActorGrabbing = nullptr;

	// Debug
	UE_LOG(LogTemp, Log, TEXT("%s released"), *GetOwner()->GetName());
}
void UUGrabbableComponent::OnThrow(FVector ThrowForce)
{
	if (!RootPrimitiveComponent) return;

	// Impulse to throw object
	RootPrimitiveComponent->AddImpulse(ThrowForce, NAME_None, true);

	// Debug
	UE_LOG(LogTemp, Log, TEXT("%s thrown with force %s"), *GetOwner()->GetName(), *ThrowForce.ToCompactString());
}
float UUGrabbableComponent::GetObjectMass() const
{
	if (!RootPrimitiveComponent) return 0.0f;

	// Return the mass
	return RootPrimitiveComponent->GetMass();
}

// Helper Functions
UPrimitiveComponent* UUGrabbableComponent::GetRootPrimitive() const
{
	if (const AActor* Owner = GetOwner())
	{
		return Cast<UPrimitiveComponent>(Owner->GetRootComponent());
	}
	return nullptr;
}

