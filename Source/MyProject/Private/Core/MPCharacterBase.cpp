// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MPCharacterBase.h"

// Sets default values
AMPCharacterBase::AMPCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

