// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

//
class UCameraComponent;
class UUDetectionComponent;
struct FVisionDetectionInfo;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

	//
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TryInteract(const FVisionDetectionInfo& DetectionInfo);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	//
	UPROPERTY()
	TObjectPtr<UUDetectionComponent> DetectionComponent;

	//
	UFUNCTION()
	void HandleVisionDetection(const FVisionDetectionInfo& VisionDetection);
};
