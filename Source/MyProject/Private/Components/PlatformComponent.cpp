// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PlatformComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/PlatformActivatableInterface.h"

// Sets default values for this component's properties
UPlatformComponent::UPlatformComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPlatformComponent::BeginPlay()
{
	Super::BeginPlay();

    StartRelativeLocation = GetRelativeLocation();
    MoveOffsetNorm = MoveOffset.GetSafeNormal();
    MaxDistance = MoveOffset.Size();

    if (ActivationMode != EActivationMode::Automatic)
    {
        TArray<UActorComponent*> Colliders = GetOwner()->GetComponentsByTag(UPrimitiveComponent::StaticClass(), "PlatformTrigger");
        if (Colliders.Num() > 0)
        {
            TriggerCollider = Cast<UPrimitiveComponent>(Colliders[0]);
            if (TriggerCollider)
            {
                TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &UPlatformComponent::OnOverlapBegin);
                TriggerCollider->OnComponentEndOverlap.AddDynamic(this, &UPlatformComponent::OnOverlapEnd);
                TriggerCollider->SetCollisionProfileName("OverlapAllDynamic");
            }
        }
    }

}

void UPlatformComponent::UpdateMovement(float DeltaTime)
{
    // set current distance
    CurDistance += DeltaTime * GetEasingSpeed() * MoveDirection;

    if (CurDistance >= MaxDistance || CurDistance <= 0.0f)
    {
        // checking if current distance is more than or equal to max distance - its at end point
        // checking if current distance is less than or equal to 0 distance - its at start point
        // if one of the conditions met - change direction of movement 
        MoveDirection *= -1;
        // ensure distance is clamped appropriately
        CurDistance = FMath::Clamp(CurDistance, 0.0f, MaxDistance);
    }
    // move platform
    SetRelativeLocation(StartRelativeLocation + MoveOffsetNorm * CurDistance);
}

void UPlatformComponent::ReverseMovement()
{
    MoveDirection *= -1; // flip movement direction
}

void UPlatformComponent::ReturnToStart(float DeltaTime)
{
    // check if platform at starting position - somewhere along designated path
    if (CurDistance > 0.0f)
    {
        // move back to StartRelativeLocation stopping once CurDistance reaches 0.0f
        CurDistance -= DeltaTime * GetEasingSpeed();
        // set max to 0 so doesnt go past the start
        CurDistance = FMath::Max(CurDistance, 0.0f);
        // move platform
        SetRelativeLocation(StartRelativeLocation + MoveOffsetNorm * CurDistance, true);
    }
}

float UPlatformComponent::GetEasingSpeed()
{
    // check for easemovement enabled or not
    if (!EaseMovement) { return Speed; } // base speed no modifiers

    // easing factor to multiply speed
    float EaseFactor = 1.0f;
    // ease in
    if (CurDistance < EaseRange)
    {
        EaseFactor = CurDistance / EaseRange;
    }
    // ease out
    else if ((MaxDistance - CurDistance) < EaseRange)
    {
        EaseFactor = (MaxDistance - CurDistance) / EaseRange;
    }
    // clamp so doesnt slow down too much
    return Speed * FMath::Clamp(EaseFactor, MinEaseFactor, 1.0f);
}

void UPlatformComponent::UpdateTrapTimers(float DeltaTime)
{
    // create actor array to store pointers to actos that will need to be removed 
    TArray<AActor*> ActorsToRemove;

    // for every element in OverlappingActors map
    for (auto& ActorPair : OverlappingActors)
    {
        // increment time
        ActorPair.Value += DeltaTime;
        // check if time is >= delay for trap to activate
        if (ActorPair.Value >= TrapDelay)
        {
            // if true, launch the actor
            LaunchActor(ActorPair.Key);
            // remove actors from map
            ActorsToRemove.Add(ActorPair.Key);
        }
    }
    // clean up launched actors
    for (AActor* Actor : ActorsToRemove)
    {
        // actors in ActorsToRemove are removed from overlapping actors
        OverlappingActors.Remove(Actor);
    }
}

void UPlatformComponent::LaunchActor(AActor* Actor)
{
    // check for existence
    if (!Actor) return;

    // calculate direction
    FVector ForceDirection = GetUpVector() + GetForwardVector();
    // normalise force direction
    ForceDirection.Normalize();
    // get force
    FVector Force = ForceDirection * FlipForce;

    // for characters
    if (ACharacter* Character = Cast<ACharacter>(Actor))
    {
        // launch character
        Character->LaunchCharacter(Force, true, true);
    }
    // for non-character actors
    else if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
    {
        // check rootcomp for physics enabled then add impulse to rootcomp
        RootComp->AddImpulse(Force, NAME_None, true);
    }
}

void UPlatformComponent::DrawDebug()
{
    if (!bEnableDebug) return;

    // convert relative locations to world space
    FVector StartPos = StartRelativeLocation;
    FVector EndPos = StartRelativeLocation + MoveOffset;

    // draw debug line and spheres
    DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Green, false, -1.0f, 0, 5.0f);
    DrawDebugSphere(GetWorld(), StartPos, 10.0f, 12, FColor::Blue, false, -1.0f);
    DrawDebugSphere(GetWorld(), EndPos, 10.0f, 12, FColor::Red, false, -1.0f);
}

void UPlatformComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) return;

    // Check if the overlapping actor implements the interface
    if (OtherActor->GetClass()->ImplementsInterface(UPlatformActivatableInterface::StaticClass()))
    {
        OverlapCount++;
        if (ActivationMode == EActivationMode::Trap)
        {
            OverlappingActors.Add(OtherActor, 0.0f);
        }
    }
}

void UPlatformComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor) return;

    if (OtherActor->GetClass()->ImplementsInterface(UPlatformActivatableInterface::StaticClass()))
    {
        // Only decrement if it was previously counted
        if (OverlapCount > 0)
            OverlapCount--;

        if (ActivationMode == EActivationMode::Trap)
        {
            OverlappingActors.Remove(OtherActor);
        }
    }
}

// Called every frame
void UPlatformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // draw debug for platform movement location
    DrawDebug();

    switch (ActivationMode)
    {
        case EActivationMode::Automatic:
            UpdateMovement(DeltaTime);
        break;
        case EActivationMode::Overlap:
            if (OverlapCount > 0)
            {
                UpdateMovement(DeltaTime);
            }
            else
            {
                ReturnToStart(DeltaTime);
            }
        break;
        case EActivationMode::Trap:
            if (OverlapCount > 0)
            {
                UpdateMovement(DeltaTime);
                UpdateTrapTimers(DeltaTime);
            }
            else
            {
                ReturnToStart(DeltaTime);
            }
        break;
    }
}
