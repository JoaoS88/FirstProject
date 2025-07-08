// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UHighlightComponent.h"
#include "Components/UDetectionComponent.h"
#include "Components/UHighlightableComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UUHighlightComponent::UUHighlightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UUHighlightComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (!Camera)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraComponent missing on %s"), *GetOwner()->GetName());
	}
	DetectionComponent = GetOwner()->FindComponentByClass<UUDetectionComponent>();
	if (!DetectionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("DetectionComponent missing on %s"), *GetOwner()->GetName());
	}
	else
	{
		DetectionComponent->OnProximityDetected.AddDynamic(this, &UUHighlightComponent::HandleProximityDetection);
		UE_LOG(LogTemp, Log, TEXT("Added to event"));
	}
}


// Called every frame
void UUHighlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// HandleDetection
/*
void UUHighlightComponent::HandleDetection(const FDetectionInfo& Detection)
{
	UE_LOG(LogTemp, Log, TEXT("HandleDetection called with %d ValidProximityComponents"), Detection.ValidProximityComponents.Num());

	for (int32 i = 0; i < Detection.ValidProximityComponents.Num(); ++i)
	{
		UActorComponent* Comp = Detection.ValidProximityComponents[i];
		if (Comp)
		{
			FString CompName = Comp->GetName();
			FString OwnerName = Comp->GetOwner() ? Comp->GetOwner()->GetName() : TEXT("NoOwner");
			UE_LOG(LogTemp, Log, TEXT("  Component %d: %s (Owner: %s)"), i, *CompName, *OwnerName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("  Component %d: nullptr"), i);
		}
	}

	if (Detection.ValidProximityComponents.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("No proximity components detected."));

		// Clear any leftover hihglights
		CurrentlyHighlighted = MoveTemp(StillHighlighted);
		StillHighlighted.Empty();
		return;
	}

	TArray<UUHighlightableComponent*> Highlightables;
	for (UActorComponent* Comp : Detection.ValidProximityComponents)
	{
		if (UUHighlightableComponent* HighlightComp = Cast<UUHighlightableComponent>(Comp))
		{
			Highlightables.Add(HighlightComp);
		}
	}

	// Use your logic here to highlight closest N or any criteria
	HighlightClosest(Highlightables);
}*/

void UUHighlightComponent::HandleProximityDetection(const FProximityDetectionInfo& Detection)
{
	if (Detection.bEnteringProximity)
	{
		// Highlight all entering components
		for (UActorComponent* Comp : Detection.ValidComponents)
		{
			if (UUHighlightableComponent* HighlightComp = Cast<UUHighlightableComponent>(Comp))
			{

				HighlightComp->SetHighlighted(true);
				CurrentlyHighlighted.Add(HighlightComp);

				// Log object name and highlight status
				//UE_LOG(LogTemp, Log, TEXT("Object %s is now Highlighted"), *HighlightComp->GetOwner()->GetName());
			}
		}
	}
	else
	{
		// Unhighlight all exiting components
		for (UActorComponent* Comp : Detection.ValidComponents)
		{
			if (UUHighlightableComponent* HighlightComp = Cast<UUHighlightableComponent>(Comp))
			{

				HighlightComp->SetHighlighted(false);
				CurrentlyHighlighted.Remove(HighlightComp);

				// Log object name and highlight status
				//UE_LOG(LogTemp, Log, TEXT("Object %s is no longer Highlighted"), *HighlightComp->GetOwner()->GetName());
			}
		}
	}

}

//void UUHighlightComponent::HighlightClosest(const TArray<UUHighlightableComponent*> Highlightables)
//{
//	//UE_LOG(LogTemp, Log, TEXT("Trying to highlight closest"));
//
//	if (!Camera || Highlightables.Num() == 0) return;
//
//	// Sort by distance to camera
//	FVector CameraLocation = Camera->GetComponentLocation();
//
//	TArray<UUHighlightableComponent*> SortedHighlightables = Highlightables;
//	SortedHighlightables.Sort([&](const UUHighlightableComponent& A, const UUHighlightableComponent& B)
//	{
//		float DistA = FVector::DistSquared(CameraLocation, A.GetOwner()->GetActorLocation());
//		float DistB = FVector::DistSquared(CameraLocation, B.GetOwner()->GetActorLocation());
//		return DistA < DistB;
//	});
//
//	int32 NumToHighlight = FMath::Clamp(MaxHighlighted, 1, SortedHighlightables.Num());
//
//	//UE_LOG(LogTemp, Log, TEXT("Highlighting %d components"), NumToHighlight);
//
//	for (int32 i = 0; i < NumToHighlight; ++i)
//	{
//		if (SortedHighlightables[i])
//		{
//			SortedHighlightables[i]->SetHighlighted(true);
//			StillHighlighted.Add(SortedHighlightables[i]);
//		}
//	}
//
//	// Turn off highlight for everything not in the new set
//	for (UUHighlightableComponent* PreviouslyHighlighted : CurrentlyHighlighted)
//	{
//		if (!StillHighlighted.Contains(PreviouslyHighlighted))
//		{
//			PreviouslyHighlighted->SetHighlighted(false);
//		}
//	}
//
//	CurrentlyHighlighted = MoveTemp(StillHighlighted);
//	StillHighlighted.Empty();
//
//}
