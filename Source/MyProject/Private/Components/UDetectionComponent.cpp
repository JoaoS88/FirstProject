// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UDetectionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UUDetectionComponent::UUDetectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // Volume used for detecting nearby objects
    ProximityVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityVolume"));
	
 
}

void UUDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

    // Check owner
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Error, TEXT("DetectionComponent has no owner"));
        return;
    }
    // Get Root of Owner
    USceneComponent* RootScene = Owner->GetRootComponent();
    if (RootScene)
    {
        ProximityVolume->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
        ProximityVolume->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
        ProximityVolume->SetSphereRadius(MaxDetectionRadius);
        ProximityVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        ProximityVolume->SetCollisionObjectType(ECC_WorldDynamic);
        ProximityVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
        ProximityVolume->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
        ProximityVolume->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
        ProximityVolume->SetGenerateOverlapEvents(true);

        ProximityVolume->RegisterComponent(); // Register Component so its detected

        // Bind overlap events
        ProximityVolume->OnComponentBeginOverlap.AddDynamic(this, &UUDetectionComponent::OnBeginOverlap);
        ProximityVolume->OnComponentEndOverlap.AddDynamic(this, &UUDetectionComponent::OnEndOverlap);
    }

    // Find Components
    Camera = Owner->FindComponentByClass<UCameraComponent>();
    if (!Camera)
    {
        UE_LOG(LogTemp, Warning, TEXT("CameraComponent missing on %s"), *GetOwner()->GetName());
    }

}
void UUDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    //UpdateProximity();
}

// Getting Valid Components
TArray<UActorComponent*> UUDetectionComponent::GetValidComponents(AActor* Actor, const TArray<TSubclassOf<UActorComponent>>& Filters)
{
    TArray<UActorComponent*> Result;

    if (!Actor || Filters.Num() == 0) return Result;

    const TSet<UActorComponent*>& AllComponents = Actor->GetComponents();

    //UE_LOG(LogTemp, Log, TEXT("Checking components on Actor: %s"), *Actor->GetName());

    for (UActorComponent* Comp : AllComponents)
    {
        if (!Comp) continue;

        //UE_LOG(LogTemp, Log, TEXT("  Component: %s, Class: %s"), *Comp->GetName(), *Comp->GetClass()->GetName());

        for (const TSubclassOf<UActorComponent>& FilterClass : Filters)
        {
            if (FilterClass && Comp->IsA(FilterClass))
            {
                //UE_LOG(LogTemp, Log, TEXT("    -> Valid component found: %s"), *Comp->GetName());
                Result.Add(Comp);
                break;
            }
        }
    }
    return Result;
}

// Vision Detection
FVisionDetectionInfo UUDetectionComponent::PerformVisionDetection()
{
    // Line trace parameters
    FVector Start = Camera->GetComponentLocation();
    FVector End = Start + (Camera->GetForwardVector() * MaxTraceDistance);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());

    // Perform line trace
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_PhysicsBody, Params);

    // Create instance of VisionDetectionInfo
    FVisionDetectionInfo VisionDetectionInfo;


    if (!Camera)
    {
        UE_LOG(LogTemp, Warning, TEXT("PerformVisionDetection: Camera is null"));
        return VisionDetectionInfo;
    }

    // Check valid line trace 
    if (HitResult.bBlockingHit && HitResult.GetActor())
    {
        // Store line trace results in DetectionInfo struct
        VisionDetectionInfo.DetectedActor = HitResult.GetActor();
        VisionDetectionInfo.bLineTraceHit = true;
        VisionDetectionInfo.HitResult = HitResult;
        VisionDetectionInfo.ValidComponents = GetValidComponents(HitResult.GetActor(), VisionComponentFilters);
    }

    // Broadcast VisionDetectionInfo for subscribed listeners
    OnVisionDetected.Broadcast(VisionDetectionInfo);

    // Show Debug if true
    DrawDebugVisionTrace(Start, End, HitResult);

    return VisionDetectionInfo;
}

// Proximity Detection
/*void UUDetectionComponent::UpdateProximity()
{
    if (!ProximityVolume)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateProximity: ProximityVolume is null"));
        return;
    }

    TArray<AActor*> CurrentOverlappingActors;
    ProximityVolume->GetOverlappingActors(CurrentOverlappingActors);

    UE_LOG(LogTemp, Log, TEXT("UpdateProximity running, overlapping actors: %d"), CurrentOverlappingActors.Num());

    TSet<AActor*> CurrentSet(CurrentOverlappingActors);

    // Actors entered = Current - Previous
    TSet<AActor*> Entered = CurrentSet.Difference(PreviousOverlappingActors);
    // Actors exited = Previous - Current
    TSet<AActor*> Exited = PreviousOverlappingActors.Difference(CurrentSet);

    for (AActor* Actor : Entered)
    {
        if (!IsValid(Actor) || Actor == GetOwner()) continue;

        TArray<UActorComponent*> ValidComponents = GetValidComponents(Actor, ProximityComponentFilters);
        if (ValidComponents.Num() == 0)
        {
            //UE_LOG(LogTemp, Log, TEXT("Entered Actor %s has no valid components for proximity."), *Actor->GetName());
            continue;
        }

        //UE_LOG(LogTemp, Log, TEXT("[Proximity] Entered: %s"), *Actor->GetName());

        FProximityDetectionInfo Info;
        Info.DetectedActors.Add(Actor);
        Info.ValidComponents = ValidComponents;
        Info.bEnteringProximity = true;

        OnProximityDetected.Broadcast(Info);

        //if (bShowDebug)
        //{
           // DrawDebugProximityTrace({ Actor });
        //}
    }

    for (AActor* Actor : Exited)
    {
        if (!IsValid(Actor) || Actor == GetOwner()) continue;

        //UE_LOG(LogTemp, Log, TEXT("[Proximity] Exited: %s"), *Actor->GetName());

        FProximityDetectionInfo Info;
        Info.DetectedActors.Add(Actor);
        Info.ValidComponents.Empty();
        Info.bEnteringProximity = false;

        OnProximityDetected.Broadcast(Info);
    }

    if (bShowDebug && ProximityVolume)
    {
        TArray<AActor*> ValidActorsInProximity;

        for (AActor* Actor : CurrentSet)
        {
            if (!IsValid(Actor) || Actor == GetOwner()) continue;

            TArray<UActorComponent*> ValidComponents = GetValidComponents(Actor, ProximityComponentFilters);
            if (ValidComponents.Num() == 0)
            {
                //UE_LOG(LogTemp, Log, TEXT("Actor %s inside proximity has no valid components."), *Actor->GetName());
                continue;
            }

            //UE_LOG(LogTemp, Log, TEXT("Actor %s inside proximity with %d valid components."), *Actor->GetName(), ValidComponents.Num());

            ValidActorsInProximity.Add(Actor);
        }

        //if (ValidActorsInProximity.Num() > 0)
        //{
            //DrawDebugProximityTrace(ValidActorsInProximity);
        //}
    }

    PreviousOverlappingActors = CurrentSet;
}
*/

// Proximity Detection
void UUDetectionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // Check if valid overlap
    if (!OtherActor || OtherActor == GetOwner()) return;

    // Filter components and Check for presence
    TArray<UActorComponent*> ValidComponents = GetValidComponents(OtherActor, ProximityComponentFilters);
    if (ValidComponents.Num() == 0) return;

    // Create instance of ProximityDetectionInfo
    FProximityDetectionInfo ProximityDetectionInfo;

    // Add valid component
    ProximityDetectionInfo.ValidComponents.Append(ValidComponents);
    // Add valid overlapped Actor
    ProximityDetectionInfo.DetectedActors.Add(OtherActor);
    // Set EnteringProximity to true
    ProximityDetectionInfo.bEnteringProximity = true;

    // Broadcast ProximityDetectionInfo for subscribed listeners
    OnProximityDetected.Broadcast(ProximityDetectionInfo);
}
void UUDetectionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    // Check if valid overlap
    if (!OtherActor || OtherActor == GetOwner()) return;

    // Filter components
    TArray<UActorComponent*> ValidComponents = GetValidComponents(OtherActor, ProximityComponentFilters);
    if (ValidComponents.Num() == 0) return;

    // Create instance of ProximityDetectionInfo
    FProximityDetectionInfo ProximityDetectionInfo;

    ProximityDetectionInfo.DetectedActors = { OtherActor };
    // Send Valid Components still so can be unhighlighted
    ProximityDetectionInfo.ValidComponents = ValidComponents;
    // Set EnteringProximity to false
    ProximityDetectionInfo.bEnteringProximity = false;

    // Broadcast ProximityDetectionInfo for subscribed listeners
    OnProximityDetected.Broadcast(ProximityDetectionInfo);
}

// Debug
void UUDetectionComponent::DrawDebugVisionTrace(FVector Start, FVector End, FHitResult HitResult) const
{
    if (!bShowDebug) return;

    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 2.0f);

    if (HitResult.bBlockingHit)
    {
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Red, false, 1.0f);
    }
}
void UUDetectionComponent::DrawDebugProximityTrace(const TArray<AActor*>& DetectedActors) const
{
    if (!bShowDebug || !ProximityVolume) return;

    /*FVector Center = ProximityVolume->GetComponentLocation();
    DrawDebugSphere(GetWorld(), Center, MaxDetectionRadius, 32, FColor::Blue, false, 5.0f, 0, 2.0f);*/

    for (AActor* Actor : DetectedActors)
    {
        if (!Actor) continue;
        DrawDebugSphere(GetWorld(), Actor->GetActorLocation(), 100.0f, 1, FColor::Red, 1.0f, false, 1.0f);
    }
}
