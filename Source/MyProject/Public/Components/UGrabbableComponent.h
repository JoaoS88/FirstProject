// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UGrabbableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UUGrabbableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUGrabbableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Cached Object References
	UPROPERTY(VisibleAnywhere, Category = "Grab")
	TObjectPtr<AActor> ActorGrabbing = nullptr;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> RootPrimitiveComponent = nullptr;

public:	
	// Functionality
	void OnGrab(AActor* Grabber);	
	void OnRelease(bool bFreeze);
	void OnThrow(FVector ThrowForce);
	float GetObjectMass() const;

private:
	// Helper Functions
	UPrimitiveComponent* GetRootPrimitive() const;
	
};
