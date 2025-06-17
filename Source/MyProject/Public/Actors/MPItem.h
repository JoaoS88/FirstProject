// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IGrabbable.h"
#include "MPItem.generated.h"

UCLASS()
class MYPROJECT_API AMPItem : public AActor, public IIGrabbable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPItem();

	UPROPERTY(EditAnywhere) 
	TObjectPtr<UPrimitiveComponent> GrabbableMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Grab_Implementation(AActor* Grabber) override;
	virtual void Release_Implementation(bool bFreeze) override;
	virtual void Throw_Implementation(FVector ThrowForce) override;
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	virtual float GetObjectMass_Implementation() const override;
};
