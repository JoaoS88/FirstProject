// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MPItem.h"

// Sets default values
AMPItem::AMPItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    GrabbableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrabbableMesh"));
    SetRootComponent(GrabbableMesh);

    GrabbableMesh->SetSimulatePhysics(true);
    GrabbableMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
}

// Called when the game starts or when spawned
void AMPItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMPItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMPItem::Grab_Implementation(AActor* Grabber)
{
    
}

void AMPItem::Release_Implementation(bool bFreeze)
{
    if (GrabbableMesh)
    {
        if (bFreeze)
        {
            GrabbableMesh->SetSimulatePhysics(false);  // Freezes in place
        }
        else
        {
            GrabbableMesh->SetSimulatePhysics(true);   // Drops normally
        }
        GrabbableMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    }
}

void AMPItem::Throw_Implementation(FVector ThrowForce)
{
    if (GrabbableMesh)
    {
        GrabbableMesh->SetSimulatePhysics(true);
        GrabbableMesh->AddImpulse(ThrowForce, NAME_None, true);
    }
}

void AMPItem::Interact_Implementation(AActor* InteractingActor)
{
    UE_LOG(LogTemp, Warning, TEXT("Interacting with Grabbable Object"));
}

float AMPItem::GetObjectMass_Implementation() const
{
    return GrabbableMesh ? GrabbableMesh->GetMass() : 1.f;
}

