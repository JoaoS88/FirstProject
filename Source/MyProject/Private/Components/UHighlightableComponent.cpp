// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UHighlightableComponent.h"

// Sets default values for this component's properties
UUHighlightableComponent::UUHighlightableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UUHighlightableComponent::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Log, TEXT("UUHighlightableComponent BeginPlay called on %s"), *GetOwner()->GetName());

	if (!TargetMesh)
	{
		// Try to find a mesh automatically
		TargetMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	}
	if(TargetMesh)
	{
		// Cache the original material from the first element
		OriginalMaterial = TargetMesh->GetMaterial(0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HighlightableComponent: No TargetMesh found on %s"), *GetOwner()->GetName());
	}
}

void UUHighlightableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UUHighlightableComponent::SetHighlighted(bool bEnable)
{
	//UE_LOG(LogTemp, Log, TEXT("SetHighlighted called with %s"), bEnable ? TEXT("true") : TEXT("false"));

	if (bIsHighlighted == bEnable || !TargetMesh) return; // No change

	bIsHighlighted = bEnable;

	// Swap materials
	if (bEnable)
	{
		if (HighlightMaterial)
		{
			TargetMesh->SetMaterial(0, HighlightMaterial);
		}
	}
	else
	{
		if (OriginalMaterial)
		{
			TargetMesh->SetMaterial(0, OriginalMaterial);
		}
	}

	// Debug
	FString StatusText = FString::Printf(TEXT("%s is now %s"),
		*GetOwner()->GetName(),
		bEnable ? TEXT("Highlighted") : TEXT("Unhighlighted"));

	// Show on screen
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,             // Key (-1 means add new message)
			2.0f,           // Display time (seconds)
			bEnable ? FColor::Green : FColor::Red,
			StatusText
		);
	}
	UE_LOG(LogTemp, Log, TEXT("%s is now %s"), *GetOwner()->GetName(), bEnable ? TEXT("Highlighted") : TEXT("Unhighlighted"));
}


