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
	UCameraComponent* Camera; 	// Camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm; // SpringArm
	//
	// --Input--
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
	//
	
	// --Movement--
	// ?could move this to the character movement component - need to look at how to do this?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerMovement")
	float DefaultWalkSpeed = 600.0f;
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
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void RunStarted(const FInputActionValue& Value);
	void RunEnded(const FInputActionValue& Value);
	void CrouchStarted(const FInputActionValue& Value);
	void CrouchEnded(const FInputActionValue& Value);
	void JumpStarted(const FInputActionValue& Value);
	void JumpEnded(const FInputActionValue& Value);

	void DebugMessage(const FString& Message);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebug = false;
	//

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
