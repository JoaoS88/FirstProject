// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGrabbable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIGrabbable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API IIGrabbable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	// When object is grabbed
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	void Grab(AActor* Grabber);

	// When object is released
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	void Release(bool bFreeze);

	// When object is thrown 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	void Throw(FVector ThrowForce);

	// When held object is interacted with 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	void Interact(AActor* InteractingActor);

	// Getting the object's mass for weight-based movement 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	float GetObjectMass() const;
};
