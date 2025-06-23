// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MPCharacterBase.generated.h"

struct FInputActionValue;
// Forward Declarations
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UPhysicsHandleComponent;
class UGrabComponent;

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
	// ---Interaction---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> GrabAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ThrowAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FreezeObjectAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AdjustDistanceAction;
	//
	
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

	// --Interacting--
	// ?could move this into character interaction component to control interaction - need to look at doing this?
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MaxGrabMass = 150.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MaxGrabDistance = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MinHoldDistance = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MaxHoldDistance = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float BaseThrowForce = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float RotationSpeed = 1.25f;*/
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
	// ---Interaction---
	void GrabStarted();
	void GrabEnded();
	void ThrowObject();
	void FreezeObject();
	void AdjustObjectDistance(const FInputActionValue& Value);
	
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

	UGrabComponent* GrabComponent = nullptr;

};
