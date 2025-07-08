// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IInteractable.h"
#include "MPDoorBase.generated.h"

UCLASS()
class MYPROJECT_API AMPDoorBase : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPDoorBase();

	// Interface Implementation
	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractText_Implementation() const override;
	virtual bool CanInteract_Implementation() const override;

	// Control Door state
	UFUNCTION(BlueprintCallable, Category = "Door")
	virtual void ToggleDoor(AActor* Interactor = nullptr);

	// Sound effect to play when door toggles
	UPROPERTY(EditDefaultsOnly, Category = "Door|Effects")
	USoundBase* DoorSound;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FrameMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Properties
	UPROPERTY(EditAnywhere, Category = "Door")
	bool bStartOpen = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsOpen = false;

	// Blueprint events
	UFUNCTION(BlueprintNativeEvent, Category = "Door")
	void OnDoorOpened();
	UFUNCTION(BlueprintNativeEvent, Category = "Door")
	void OnDoorClosed();

	// Control Door Effects
	UFUNCTION(BlueprintNativeEvent, Category = "Door")
	void PlayDoorEffects(AActor* Interactor);


private:
	// Ensure door's visual state matches logical state
	void InitializeDoor();
};
