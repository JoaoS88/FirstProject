// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UHighlightComponent.generated.h"

//
class UCameraComponent;

// Detection Components
class UUDetectionComponent;
struct FProximityDetectionInfo;
// Highlightable Actor Component
class UUHighlightableComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UUHighlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUHighlightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	//
	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY()
	TObjectPtr<UUDetectionComponent> DetectionComponent;

	//
	UPROPERTY()
	TSet<TObjectPtr<UUHighlightableComponent>> CurrentlyHighlighted;

	TSet<TObjectPtr<UUHighlightableComponent>> StillHighlighted;

	//
	/*UPROPERTY(EditDefaultsOnly, Category = "Highlight", meta = (AllowPrivateAccess = "true"))
	int32 MaxHighlighted = 3;*/

	//
	UFUNCTION()
	void HandleProximityDetection(const FProximityDetectionInfo& Detection);

	//void HighlightClosest(const TArray<UUHighlightableComponent*> Highlightables);
};
