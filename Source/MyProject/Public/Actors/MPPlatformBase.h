// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPPlatformBase.generated.h"

class UPlatformComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class MYPROJECT_API AMPPlatformBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPPlatformBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
	TObjectPtr<UPlatformComponent> PlatformComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
	TObjectPtr<UBoxComponent> TriggerBox;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
