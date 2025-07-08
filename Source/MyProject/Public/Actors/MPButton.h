// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MPActivatorBase.h"
#include "MPButton.generated.h"

UCLASS()
class MYPROJECT_API AMPButton : public AMPActivatorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMPButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when interaction happens
	virtual void PlayActivationEffects_Implementation(AActor* Interactor) override;
	virtual FText GetInteractText_Implementation() const override;

	// How far button depresses when pressed 
	UPROPERTY(EditAnywhere, Category = "Button")
	float PressDepth = -10.0f;

	// Initial location position of the mesh
	UPROPERTY()
	FVector InitialLocation;
};
