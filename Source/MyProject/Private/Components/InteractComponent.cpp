// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractComponent.h"
#include "Components/UDetectionComponent.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/IInteractable.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	//
	DetectionComponent = GetOwner()->FindComponentByClass<UUDetectionComponent>();
	if (!DetectionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("DetectionComponent missing on %s"), *GetOwner()->GetName());
	}
	else
	{
		DetectionComponent->OnVisionDetected.AddDynamic(this, &UInteractComponent::HandleVisionDetection);
		UE_LOG(LogTemp, Log, TEXT("Added to event"));	
	}	
}

void UInteractComponent::TryInteract(const FVisionDetectionInfo& DetectionInfo)
{
	if (!DetectionInfo.bLineTraceHit || !DetectionInfo.DetectedActor) return;

	if (DetectionInfo.DetectedActor->Implements<UIInteractable>())
	{
		IIInteractable::Execute_OnInteract(DetectionInfo.DetectedActor, GetOwner());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor is not interactable."));
	}
}

void UInteractComponent::HandleVisionDetection(const FVisionDetectionInfo& VisionDetection)
{
	//
}