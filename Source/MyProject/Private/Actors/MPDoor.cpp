// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MPDoor.h"

// Sets default values
AMPDoor::AMPDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// getting mesh component MeshComp
	// creating UStaticMeshComponent
	// give it a name "Mesh Component"
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	// this will be created in default hierarchy - will become child component of root component
	// can make the mesh comp the root component itself
	RootComponent = MeshComp;
	// can also attack to root component using:
	// MeshComp->SetupAttatchement(RootComponent)

}

// Called when the game starts or when spawned
void AMPDoor::BeginPlay()
{
	Super::BeginPlay();

	// using the SetActorRotation to set the rotation using RotationTarget variable
	SetActorRotation(RotationTarget);
	
}

// Called every frame
void AMPDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

