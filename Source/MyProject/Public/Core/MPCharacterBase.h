// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MPCharacterBase.generated.h"

struct FInputActionValue;
// Forward Declarations
// Input
class UInputMappingContext;
class UInputAction;
// Character
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class USphereComponent;
class UPhysicsHandleComponent;
// Custom Components
class UGrabComponent;
class UUDetectionComponent;
class UInteractComponent;

UCLASS()
class MYPROJECT_API AMPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMPCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// --Components--
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera; 	// Camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent > SpringArm; // SpringArm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics")
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle; // PhysicsHandle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> ProximityVolume;
	//
	
	// --Input--
	// ---Movement---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveActions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookActions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;
	// ---Grab---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> GrabAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ThrowAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FreezeObjectAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AdjustDistanceAction;
	// ---Interact---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;
	
	// --Movement--
	// ?could move this to the character movement component - need to look at how to do this?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMovement")
	float BaseWalkSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMovement")
	float RunSpeedMultiplier = 1.75f;
	//

	// --Looking--
	// ?could move this into a character camera component to control camera - need to look at doing this?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MouseMovement")
	float LookSensitivityX = 0.85f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MouseMovement")
	float LookSensitivityY = 0.85f;
	//

	// --Functions--
	// ---Movement---
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void RunStarted();
	void RunEnded();
	void CrouchStarted();
	void CrouchEnded();
	void JumpStarted();
	void JumpEnded();
	// ---Grab---
	void GrabStarted();
	void GrabEnded();
	void ThrowObject();
	void FreezeObject();
	void AdjustObjectDistance(const FInputActionValue& Value);
	// ---Interact---
	void Interact();

	// ---Debug---
	void DebugMessage(const FString& Message);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebug = false;
	//

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// Component References
	TObjectPtr<UUDetectionComponent> DetectionComponent = nullptr;
	TObjectPtr<UGrabComponent> GrabComponent = nullptr;
	TObjectPtr<UInteractComponent> InteractComponent = nullptr;
};
