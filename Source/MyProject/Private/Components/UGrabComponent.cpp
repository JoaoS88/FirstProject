// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/UGrabComponent.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/UGrabbableComponent.h"


// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	// Find Components in owner that are required for grabbing logic
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandleComponent missing on %s"), *GetOwner()->GetName());
	}
	Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (!Camera)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraComponent missing on %s"), *GetOwner()->GetName());
	}
	
}

// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update Held Object Location
	if (HasValidHeldObject())
	{
		UpdateHeldObject();
	}
}

// Functionality
bool UGrabComponent::GrabIfValid(const FHitResult& HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	UPrimitiveComponent* HitComp = HitResult.GetComponent();

	// Check if hit result has valid actor, component and physics handle
	if (!HitActor || !HitComp || !PhysicsHandle) return false;

	// Try finding UGrabbableComponent on hit actor to confirm if grabbable or not
	UUGrabbableComponent* Grabbable = HitActor->FindComponentByClass<UUGrabbableComponent>();
	if (!Grabbable) return false;

	// Ensure hit component simulates physics before grabbing
	if (!HitComp->IsSimulatingPhysics())
	{
		HitComp->SetSimulatePhysics(true);
	}

	// Get Mass
	ObjectMass = Grabbable->GetObjectMass();
	// Reject grab if object's mass exceeds maximum allowed
	if (ObjectMass > MaxGrabMass) return false;

	// Store Hit Components Current Rotation
	CurrentHoldRotation = HitComp->GetComponentRotation();

	// Grab object using Physics Handle
	PhysicsHandle->GrabComponentAtLocationWithRotation(
		HitComp,
		NAME_None,
		HitResult.ImpactPoint,
		CurrentHoldRotation
	);

	// Component Implementation
	Grabbable->OnGrab(GetOwner());

	// Cache 
	HeldObject = HitActor;
	GrabbedComponent = HitComp;
	GrabbableComponent = Grabbable;

	CurrentHoldDistance = FMath::Clamp(
		FVector::Dist(Camera->GetComponentLocation(), HitComp->GetComponentLocation()),
		MinHoldDistance,
		MaxHoldDistance
	);

	return true;
}
void UGrabComponent::TryGrabObject()
{
	if (!IsReadyForGrab()) return;

	// Line trace parameters
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * MaxGrabDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	// Perform line trace
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_PhysicsBody, Params);
	
	if (bHit && !GrabIfValid(HitResult))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor is not grabbable or too heavy."));
	}

	// If debug on then show line trace debug line
	DrawDebugTrace(Start, End, HitResult);
}
void UGrabComponent::ReleaseObject(bool bFreeze)
{
	if (!HasValidHeldObject()) return;

	// Release from Physics Handle
	if (HasValidPhysicsHandle())
	{
		PhysicsHandle->ReleaseComponent();
	}

	if (GrabbableComponent)
	{
		// Execute object release from component
		GrabbableComponent->OnRelease(bFreeze);
	}

	ResetHeldObject();
}
void UGrabComponent::ThrowObject()
{
	if (!HasValidHeldObject() || !Camera) return;

	// Release from Physics Handle
	if (HasValidPhysicsHandle())
	{
		PhysicsHandle->ReleaseComponent();
	}

	if (GrabbableComponent)
	{
		// Get Mass
		ObjectMass = FMath::Max(ObjectMass, 1.0f);
		// Calculate Throw Force and execute object throw from component
		FVector ThrowForce = Camera->GetForwardVector() * (BaseThrowForce / (ObjectMass/ 100.0f));
		GrabbableComponent->OnThrow(ThrowForce);
	}

	ResetHeldObject();
}
void UGrabComponent::AdjustObjectDistance(float ScrollDelta)
{
	// Zoom in/out depending on scroll direction
	CurrentHoldDistance = FMath::Clamp(
		CurrentHoldDistance + ScrollDelta * ScrollSpeed,
		MinHoldDistance,
		MaxHoldDistance
	);
}
void UGrabComponent::UpdateHeldObject()
{
	if (!HasValidHeldObject() || !Camera || !PhysicsHandle) return;

	FVector TargetLocation = Camera->GetComponentLocation() + Camera->GetForwardVector() * CurrentHoldDistance;
	FRotator HoldRotation = GrabbedComponent->GetComponentRotation();
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, CurrentHoldRotation);
}

// Helper Functions
bool UGrabComponent::IsReadyForGrab() const
{
	return PhysicsHandle != nullptr && Camera != nullptr;
}
bool UGrabComponent::HasValidHeldObject() const
{
	return HeldObject != nullptr && GrabbedComponent != nullptr;
}
bool UGrabComponent::HasValidPhysicsHandle() const
{
	return PhysicsHandle && PhysicsHandle->GrabbedComponent;
}
void UGrabComponent::ResetHeldObject()
{
	HeldObject = nullptr;
	GrabbedComponent = nullptr;
	GrabbableComponent = nullptr;
	CurrentHoldDistance = 0.f;
	ObjectMass = 0.f;
	CurrentHoldRotation = FRotator::ZeroRotator;
}

// Debug
void UGrabComponent::DrawDebugTrace(FVector Start, FVector End, FHitResult HitResult) const
{
	if (!bShowDebug) return;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 2.0f);

	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Red, false, 1.0f);
	}
}