// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MPActivatorBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AMPActivatorBase::AMPActivatorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMPActivatorBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Create Static Mesh for activator
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

void AMPActivatorBase::OnInteract_Implementation(AActor* Interactor)
{
	bIsActive = !bIsActive;

	// Broadcast to any registered listeners
	OnActivated.Broadcast(Interactor);

	// Trigger effects
	PlayActivationEffects(Interactor);

	// Base activation logic
	for (AActor* Target : TargetActors)
	{
		if (Target && Target->Implements<UIInteractable>())
		{
			IIInteractable::Execute_OnInteract(Target, Interactor);
		}
	}
}
FText AMPActivatorBase::GetInteractText_Implementation() const
{
	// Base implementation 
	return FText::FromString(bIsActive ? TEXT("Deactivate") : TEXT("Activate"));
}
bool AMPActivatorBase::CanInteract_Implementation() const
{
	// Base implementation
	// Can always interact unless subclass says otherwise
	return true;			
}

//
void AMPActivatorBase::PlayActivationEffects_Implementation(AActor* Interactor)
{
	// Base implementation 
	// Placeholder debug, subclass will override
	UE_LOG(LogTemp, Log, TEXT("%s triggered activation by %s"), *GetName(), *GetNameSafe(Interactor));
	// Play added sound from blueprint if needed
	if (ActivationSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ActivationSound, GetActorLocation());
	}
}
