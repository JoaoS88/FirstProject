// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MPPlatformBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/UPlatformComponent.h"

// Sets default values
AMPPlatformBase::AMPPlatformBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the PlatformComponent as root
	PlatformComponent = CreateDefaultSubobject<UPlatformComponent>(TEXT("PlatformComponent"));
	RootComponent = PlatformComponent;

	// Attach Mesh to PlatformComponent root
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	Mesh->SetupAttachment(PlatformComponent);

	// Attach TriggerBox to Mesh
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(Mesh);
	TriggerBox->ComponentTags.Add("PlatformTrigger");
	TriggerBox->SetCollisionProfileName("OverlapAllDynamic");

}

// Called when the game starts or when spawned
void AMPPlatformBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMPPlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}




