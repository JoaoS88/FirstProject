// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IInteractable.h"
#include "MPActivatorBase.generated.h"

// Delegate to broadcast when activator triggered
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivatorTriggeredSignature, AActor*, Interactor);

UCLASS()
class MYPROJECT_API AMPActivatorBase : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPActivatorBase();

	// Event triggered when activator used
	UPROPERTY(BlueprintAssignable, Category = "Activator|Events")
	FOnActivatorTriggeredSignature OnActivated;

	// Reference to actors this activator should activate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activator|Targets")
	TArray<AActor*> TargetActors;

	// Sound to play when activated
	UPROPERTY(EditDefaultsOnly, Category = "Activator|Effects")
	USoundBase* ActivationSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Interface implementation
	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractText_Implementation() const override;
	virtual bool CanInteract_Implementation() const override;

	// Called when activator is triggered
	UFUNCTION(BlueprintNativeEvent, Category = "Activator")
	void PlayActivationEffects(AActor* Interactor);

	// Static Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	// State tracking
	bool bIsActive = false;

};
