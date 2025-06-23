// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UGrabComponent.generated.h"

class UCameraComponent;
class UPhysicsHandleComponent;
class UUGrabbableComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UGrabComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Functionality
	// (currently not exposed, but possibly in future if needed to use in blueprints for ui, could do)
	void TryGrabObject();
	void ReleaseObject(bool bFreeze = false);
	void ThrowObject();
	void AdjustObjectDistance(float ScrollDelta);

private:

	// Component References
	UPROPERTY()
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle;

	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;

	// Cached Object References
	UPROPERTY()
	TObjectPtr<AActor> HeldObject = nullptr;
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> GrabbedComponent = nullptr;
	UPROPERTY()
	TObjectPtr<UUGrabbableComponent> GrabbableComponent = nullptr;

	// Configurable Component Parameters
	UPROPERTY(EditDefaultsOnly, Category = "Grabbing", meta = (AllowPrivateAccess = "true"))
	float MaxGrabDistance = 300.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Grabbing", meta = (AllowPrivateAccess = "true"))
	float MinHoldDistance = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Grabbing", meta = (AllowPrivateAccess = "true"))
	float MaxHoldDistance = 400.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Grabbing", meta = (AllowPrivateAccess = "true"))
	float MaxGrabMass = 300.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Grabbing", meta = (AllowPrivateAccess = "true"))
	float BaseThrowForce = 1000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Grabbing", meta = (AllowPrivateAccess = "true"))
	float ScrollSpeed = 20.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Grabbing", meta = (AllowPrivateAccess = "true"))
	bool bShowDebug = false;

	// Internal Variables
	float CurrentHoldDistance;
	FRotator CurrentHoldRotation;
	float ObjectMass;

	// Internal Functions
	bool GrabIfValid(const FHitResult& HitResult);
	void UpdateHeldObject();

	// Helper Functions
	bool IsReadyForGrab() const;
	bool HasValidHeldObject() const;
	bool HasValidPhysicsHandle() const;
	void ResetHeldObject();
	
	// Debug Functions
	void DrawDebugTrace(FVector Start, FVector End, FHitResult HitResult) const;
};
