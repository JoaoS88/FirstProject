// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MPDoorBase.h"
#include "TimerManager.h"
#include "MPSlidingDoor.generated.h"

UCLASS()
class MYPROJECT_API AMPSlidingDoor : public AMPDoorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMPSlidingDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when door opens/closes
	void OnDoorOpened_Implementation() override;
	void OnDoorClosed_Implementation() override;

	// Position of door when closed
	UPROPERTY(BlueprintReadOnly, Category = "Sliding Door")
	FVector ClosedPosition;
	// Position of door when open
	UPROPERTY(BlueprintReadOnly, Category = "Sliding Door")
	FVector OpenPosition;

	// Offset applied to door's closed position to determine opened position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	FVector OpenOffset = FVector(0, 0, 200);

	// Movement duration between open and closed states
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	float SlideTime = 1.0f;
};
