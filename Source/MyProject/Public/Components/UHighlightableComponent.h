// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UHighlightableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class MYPROJECT_API UUHighlightableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUHighlightableComponent();

protected:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Highlight material to apply when highlighted
	UPROPERTY(EditAnywhere, Category = "Highlighting")
	UMaterialInterface* HighlightMaterial;

	// Mesh to apply the material to
	UPROPERTY(EditAnywhere, Category = "Highlighting")
	UStaticMeshComponent* TargetMesh;

public:	

	void SetHighlighted(bool bEnable);
	bool IsHighlighted() const { return bIsHighlighted; }

private:

	// Boolean for if highlighted or not
	bool bIsHighlighted = false;
	// Store the original material so can revert back
	UMaterialInterface* OriginalMaterial;
};
