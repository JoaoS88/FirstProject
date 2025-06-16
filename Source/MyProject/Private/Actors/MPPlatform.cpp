// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MPPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PlatformComponent.h"

// Sets default values
AMPPlatform::AMPPlatform()
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

	// Collision
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Mesh);
	CollisionBox->SetCollisionProfileName("OverlapAllDynamic");

}

void AMPPlatform::OnPlatformOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap detected! Reversing movement."));

	if (PlatformComponent)
	{
		PlatformComponent->ReverseMovement();
	}
}

// Called when the game starts or when spawned
void AMPPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMPPlatform::OnPlatformOverlap);
	}

}

// Called every frame
void AMPPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




