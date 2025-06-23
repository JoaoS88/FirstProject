// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UPlatformComponent.generated.h"

UENUM(BlueprintType)
enum class EActivationMode : uint8
{
	Automatic, // always moving with no intervention
	Overlap,   // moves when overlapped
	Trap,	   // moves and after overlap delay throws off
};


UCLASS( ClassGroup=(Platform), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UPlatformComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlatformComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Functionality
	// activation mode from enum
	UPROPERTY(EditAnywhere, Category = "Movement")
	EActivationMode ActivationMode = EActivationMode::Overlap;

	// Debug
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bEnableDebug = false;

	// Movement Parameters
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector MoveOffset;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool EaseMovement = true;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float EaseRange = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MinEaseFactor = 0.2f;

	// Trap Properties
	UPROPERTY(EditAnywhere, Category = "Trap", meta = (EditCondition = "ActivationMode == EActivationMode::Trap"))
	float TrapDelay = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Trap", meta = (EditCondition = "ActivationMode == EActivationMode::Trap"))
	float FlipForce = 1000.0f;

private:
	// Movement State
	FVector StartRelativeLocation;
	FVector MoveOffsetNorm;
	float MaxDistance = 0.0f;
	float CurDistance = 0.0f;
	int MoveDirection = 1;

	// Track Overlap
	int32 OverlapCount;
	TMap<AActor*, float> OverlappingActors; // actor and time

	// Trigger
	UPrimitiveComponent* TriggerCollider = nullptr;

	// Functions
	void UpdateMovement(float DeltaTime);
	void ReturnToStart(float DeltaTime);
	float GetEasingSpeed();
	void UpdateTrapTimers(float DeltaTime);
	void LaunchActor(AActor* Actor);
	void DrawDebug();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Functions
	UFUNCTION()
	void ReverseMovement();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
