// Fill out your copyright notice in the Description page of Project Settings.

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include "Camera/CameraComponent.h"
#include "Core/MPCharacterBase.h"


// Sets default values
AMPCharacterBase::AMPCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// --Components--
	// SpringArm - camera control
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	// Camera - camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AMPCharacterBase::Move(const FInputActionValue& Value)
{
	// 2D Vector from Value to drive movement
	FVector2D MovementVector = Value.Get<FVector2D>();

	// check vector length if not 0 - ensure one direction pressed
	if (MovementVector.Length() != 0.0f)
	{
		// move along axis
		AddMovementInput(GetActorForwardVector() * MovementVector.X);
		AddMovementInput(GetActorRightVector() * MovementVector.Y);
	}
}

void AMPCharacterBase::Look(const FInputActionValue& Value)
{
	// 2D Vector from Value to drive looking
	FVector2D LookVector = Value.Get<FVector2D>();

	if (LookVector.Length() != 0.0f)
	{
		// looking in x and y with sensitivity
		AddControllerYawInput(LookVector.X * LookSensitivityX);    // Mouse X
		AddControllerPitchInput(-LookVector.Y * LookSensitivityY);  // Mouse Y
	}
}

void AMPCharacterBase::RunStarted(const FInputActionValue& Value)
{
	// started running - set max walk speed to run speed
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * RunSpeedMultiplier;
	DebugMessage(TEXT("State: Running"));
}

void AMPCharacterBase::RunEnded(const FInputActionValue& Value)
{
	// stopped running - set max walk speed back to walk speed
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	DebugMessage(TEXT("State: Running Ended"));
}

void AMPCharacterBase::CrouchStarted(const FInputActionValue& Value)
{
	// start crouching
	Crouch();
	DebugMessage(TEXT("State: Crouching"));
}

void AMPCharacterBase::CrouchEnded(const FInputActionValue& Value)
{
	// stop crouching
	UnCrouch();
	DebugMessage(TEXT("State: Crouching Ended"));
}

void AMPCharacterBase::JumpStarted(const FInputActionValue& Value)
{
	// perform jump
	UnCrouch();
	Jump();
	DebugMessage(TEXT("State: Jumping"));
}

void AMPCharacterBase::JumpEnded(const FInputActionValue& Value)
{
	// stop jump
	StopJumping();
	DebugMessage(TEXT("State: Jumping Ended"));
}

void AMPCharacterBase::DebugMessage(const FString& Message)
{
	if (bShowDebug)
	{
		GEngine->AddOnScreenDebugMessage
		(
			1,
			5.0f,
			FColor::Green,
			Message
		);
	}
}

// Called every frame
void AMPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveActions, ETriggerEvent::Triggered, this, &AMPCharacterBase::Move);

		EnhancedInputComponent->BindAction(LookActions, ETriggerEvent::Triggered, this, &AMPCharacterBase::Look);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AMPCharacterBase::RunStarted);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMPCharacterBase::RunEnded);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMPCharacterBase::CrouchStarted);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMPCharacterBase::CrouchEnded);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMPCharacterBase::JumpStarted);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMPCharacterBase::JumpEnded);

	}

}

