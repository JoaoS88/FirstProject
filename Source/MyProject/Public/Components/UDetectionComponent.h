// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UDetectionComponent.generated.h"

// Forward Declaration - Components
class UCameraComponent;
class USphereComponent;

// Vision-specific detection data
USTRUCT(BlueprintType)
struct FVisionDetectionInfo
{
    GENERATED_BODY()

    // Line Trace Hit  - Bool
    UPROPERTY(BlueprintReadOnly)
    bool bLineTraceHit = false;

    // Result of Line Trace - FHitResult
    UPROPERTY(BlueprintReadOnly)
    FHitResult HitResult;

    // Hit Actor from Line Trace - AActor
    UPROPERTY(BlueprintReadOnly)
    AActor* DetectedActor;

    // Valid Components that result in action - TArray of ActorComponents
    UPROPERTY(BlueprintReadOnly)
    TArray<UActorComponent*> ValidComponents;
};

// Proximity-specific detection data
USTRUCT(BlueprintType)
struct FProximityDetectionInfo
{
    GENERATED_BODY()

    // Detected Actors from Proximity - TArray of AActors
    UPROPERTY(BlueprintReadOnly)
    TArray<AActor*> DetectedActors;

    // Valid Components that result in action - TArray of ActorComponents
    UPROPERTY(BlueprintReadOnly)
    TArray<UActorComponent*> ValidComponents;

    // Entering Proximity - Bool
    UPROPERTY(BlueprintReadOnly)
    bool bEnteringProximity; // True when entering, false when exiting
};

// Declaring Delegates:
// Vision Detection
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVisionDetectionSignature, const FVisionDetectionInfo&, VisionDetection);
// Proximity Detection
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProximityDetectionSignature, const FProximityDetectionInfo&, ProximityDetection);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UUDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUDetectionComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Vision Detection
    UFUNCTION(BlueprintCallable, Category = "Detection")
    FVisionDetectionInfo PerformVisionDetection();

    // Proximity Detection
    /*UFUNCTION(BlueprintCallable, Category = "Detection")
    FProximityDetectionInfo PerformProximityDetection();*/

    // Event Delegates
    UPROPERTY(BlueprintAssignable, Category = "Detection|Events")
    FVisionDetectionSignature OnVisionDetected;

    UPROPERTY(BlueprintAssignable, Category = "Detection|Events")
    FProximityDetectionSignature OnProximityDetected;

   
private:	
    
    // Components
    UPROPERTY()
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USphereComponent> ProximityVolume;

    // Configurables
    UPROPERTY(EditDefaultsOnly, Category = "Detection|Vision", meta = (AllowPrivateAccess = "true"))
    float MaxTraceDistance = 300.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Detection|Vision", meta = (AllowPrivateAccess = "true"))
    TArray<TSubclassOf<UActorComponent>> VisionComponentFilters;

    UPROPERTY(EditDefaultsOnly, Category = "Detection|Proximity", meta = (AllowPrivateAccess = "true"))
    float MaxDetectionRadius = 150.f;

    UPROPERTY(EditAnywhere, Category = "Detection|Proximity", meta = (AllowPrivateAccess = "true"))
    TArray<TSubclassOf<UActorComponent>> ProximityComponentFilters;

    UPROPERTY(EditDefaultsOnly, Category = "Detection|Debug", meta = (AllowPrivateAccess = "true"))
    bool bShowDebug = false;

    // Private Variables
    TSet<AActor*> PreviousOverlappingActors;

    // Private Functions
    TArray<UActorComponent*> GetValidComponents(AActor* Actor, const TArray<TSubclassOf<UActorComponent>>& Filters);
    //void UpdateProximity();

    // Event Functions
    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    
    // Debug Functions
    void DrawDebugVisionTrace(FVector Start, FVector End, FHitResult HitResult) const;
    void DrawDebugProximityTrace(const TArray<AActor*>& DetectedActors) const;

};
