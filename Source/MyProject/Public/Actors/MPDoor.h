// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPDoor.generated.h"

UCLASS()
class MYPROJECT_API AMPDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPDoor();

	//  * defining first component - static mesh 
	// old - UStaticMeshComponent* MeshComp;
	// new - TObjectPtr<UStaticMeshComponent> MeshComp;

	// making sure static mesh component MeshComp is visible anywhere
	// making sure static mesh is read only
	// giving the mesh a category
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh");
	TObjectPtr<UStaticMeshComponent> MeshComp;

	// FRotator variable
	// making sure we can edit RotationTarget variable anywhere
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation");
	FRotator RotationTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
