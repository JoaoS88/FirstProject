// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MPPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AMPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MovementContext, 0);
	}
}
