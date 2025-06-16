// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPPlatform.generated.h"

class UPlatformComponent;
class UStaticMeshComponent;
class UBoxComponent;


UCLASS()
class MYPROJECT_API AMPPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
	TObjectPtr<UPlatformComponent> PlatformComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
	TObjectPtr<UBoxComponent> CollisionBox;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Collision
	UFUNCTION()
	void OnPlatformOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

};
