// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneController.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

ADroneController::ADroneController() :
	InputMappingContext(nullptr),
	MoveForwardAction(nullptr),
	MoveRightAction(nullptr),
	MoveUpAction(nullptr),
	LookAction(nullptr),
	TurnRollAction(nullptr)
{
}

void ADroneController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				PlayerSubsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}