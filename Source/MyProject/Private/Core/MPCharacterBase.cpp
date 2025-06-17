// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/MPCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Interfaces/IGrabbable.h"

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
	// PhysicsHandle - interacting
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}

// Called when the game starts or when spawned
void AMPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	BaseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

// --Movement--
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
	LastLookInput = LookVector;

	// Prevent camera movement during interaction
	if (bIsInteracting && HeldObject) return;

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
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * RunSpeedMultiplier;
	DebugMessage(TEXT("State: Running"));
}
void AMPCharacterBase::RunEnded(const FInputActionValue& Value)
{
	// stopped running - set max walk speed back to walk speed
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
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

// --Interaction--
void AMPCharacterBase::GrabStarted(const FInputActionValue& Value)
{
	// Hit Result from Line trace
	FHitResult HitResult;
	// Line trace parameters
	FVector Start = Camera->GetComponentLocation();  
	FVector End = Start + (Camera->GetForwardVector() * MaxGrabDistance);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Perform line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_PhysicsBody, Params);
	// Check if Hit Result
	if (bHit)
	{
		AActor* TargetActor = HitResult.GetActor();
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		if (TargetActor && HitComponent && TargetActor->Implements<UIGrabbable>())
		{
			// Simulate Physics to be attatched to Handle
			HitComponent->SetSimulatePhysics(true);

			float ObjectMass = IIGrabbable::Execute_GetObjectMass(HitResult.GetActor());
			if (ObjectMass > MaxGrabMass)
			{
				DebugMessage(FString::Printf(TEXT("Too heavy to pick up! (%.2f kg)"), ObjectMass));
				return;
			}
			
			// Attatch to Physics Handle
			PhysicsHandle->GrabComponentAtLocation(
				HitComponent,
				NAME_None,
				HitComponent->GetComponentLocation()
			);

			
			// Execute Grab from interface
			IIGrabbable::Execute_Grab(HitResult.GetActor(), this);
			// Set Held Object
			HeldObject = TargetActor;
			// Get Hold Distance
			CurrentHoldDistance = FVector::Dist(Camera->GetComponentLocation(), HitComponent->GetComponentLocation());
		}
	}
	// Debug
	if (bShowDebug)
	{
		// Debug Line trace
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 2.0f);
		if (bHit)
		{
			// Debug Hit 
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Red, false, 1.0f);
		}
	}
}
void AMPCharacterBase::GrabEnded(const FInputActionValue& Value)
{
	if (UPrimitiveComponent* GrabbedComp = PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor = GrabbedComp->GetOwner();
		if (GrabbedActor && GrabbedActor->GetClass()->ImplementsInterface(UIGrabbable::StaticClass()))
		{
			IIGrabbable::Execute_Release(GrabbedActor, false);
		}
		PhysicsHandle->ReleaseComponent();
		DebugMessage(TEXT("State: Grabbing Ended"));
	}

}
void AMPCharacterBase::Throw(const FInputActionValue& Value)
{
	if (UPrimitiveComponent* GrabbedComp = PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor = GrabbedComp->GetOwner();
		if (GrabbedActor && GrabbedActor->GetClass()->ImplementsInterface(UIGrabbable::StaticClass()))
		{
			float ObjectMass = IIGrabbable::Execute_GetObjectMass(GrabbedActor);
			float ScaledThrowForce = BaseThrowForce * FMath::Clamp(100.f / ObjectMass, 0.5f, 5.0f);

			FVector ThrowDirection = Camera->GetForwardVector();
			FVector ThrowForce = ThrowDirection * ScaledThrowForce;

			IIGrabbable::Execute_Throw(GrabbedActor, ThrowForce);
		}
		PhysicsHandle->ReleaseComponent();
		DebugMessage(TEXT("State: Threw"));
	}
}
void AMPCharacterBase::FreezeObject(const FInputActionValue& Value)
{
	// make boolean to freeze true
	// checked when releasing object if true or false
	if (UPrimitiveComponent* GrabbedComp = PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor = GrabbedComp->GetOwner();
		if (GrabbedActor && GrabbedActor->GetClass()->ImplementsInterface(UIGrabbable::StaticClass()))
		{
			IIGrabbable::Execute_Release(GrabbedActor, true);
		}
		PhysicsHandle->ReleaseComponent();
		HeldObject = nullptr; // Reset tracking
		DebugMessage(TEXT("State: Froze Object"));
	}
}
void AMPCharacterBase::InteractStarted(const FInputActionValue& Value)
{
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		bIsInteracting = true;
		LastLookInput = FVector2D::ZeroVector;
	}
}

void AMPCharacterBase::InteractEnded(const FInputActionValue& Value)
{
	bIsInteracting = false;
}

void AMPCharacterBase::AdjustDistance(const FInputActionValue& Value)
{
	if (!HeldObject) return;

	UPrimitiveComponent* GrabbedComp = PhysicsHandle->GetGrabbedComponent();
	if (!GrabbedComp || !GrabbedComp->IsSimulatingPhysics()) return;

	float ScrollValue = Value.Get<float>();
	float Mass = IIGrabbable::Execute_GetObjectMass(HeldObject);
	float DistanceAdjustmentSpeed = FMath::Clamp(Mass / 100.0f, 0.1f, 5.0f);

	CurrentHoldDistance += ScrollValue * DistanceAdjustmentSpeed;
	CurrentHoldDistance = FMath::Clamp(CurrentHoldDistance, MinHoldDistance, MaxHoldDistance);
}

void AMPCharacterBase::RotateObject(float DeltaTime)
{
	if (!HeldObject) return;

	UPrimitiveComponent* GrabbedComp = PhysicsHandle->GetGrabbedComponent();
	if (!GrabbedComp || !GrabbedComp->IsSimulatingPhysics()) return;

	FRotator DeltaRotation = FRotator(LastLookInput.Y * RotationSpeed, LastLookInput.X * RotationSpeed, 0.f);
	GrabbedComp->AddLocalRotation(DeltaRotation);

	LastLookInput = FVector2D::ZeroVector;
}

// --Debug--
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

	if (PhysicsHandle->GetGrabbedComponent())
	{
		// Clamp distance
		float ClampedDistance = FMath::Clamp(CurrentHoldDistance, MinHoldDistance, MaxHoldDistance);

		FVector TargetLocation = Camera->GetComponentLocation() + Camera->GetForwardVector() * ClampedDistance;
		PhysicsHandle->SetTargetLocation(TargetLocation);
	}

	if (bIsInteracting && PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		RotateObject(DeltaTime);
	}

}

// Called to bind functionality to input
void AMPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// --Movement--
		EnhancedInputComponent->BindAction(MoveActions, ETriggerEvent::Triggered, this, &AMPCharacterBase::Move);
		EnhancedInputComponent->BindAction(LookActions, ETriggerEvent::Triggered, this, &AMPCharacterBase::Look);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AMPCharacterBase::RunStarted);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMPCharacterBase::RunEnded);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMPCharacterBase::CrouchStarted);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMPCharacterBase::CrouchEnded);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMPCharacterBase::JumpStarted);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMPCharacterBase::JumpEnded);

		// --Interaction--
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &AMPCharacterBase::GrabStarted);
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Completed, this, &AMPCharacterBase::GrabEnded);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &AMPCharacterBase::Throw);
		EnhancedInputComponent->BindAction(FreezeObjectAction, ETriggerEvent::Triggered, this, &AMPCharacterBase::FreezeObject);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMPCharacterBase::InteractStarted);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMPCharacterBase::InteractEnded);
		EnhancedInputComponent->BindAction(AdjustDistanceAction, ETriggerEvent::Triggered, this, &AMPCharacterBase::AdjustDistance);
	}
}

