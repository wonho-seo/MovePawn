// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessMovePawnController.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

APossessMovePawnController::APossessMovePawnController()
	: InputMappingContext(nullptr),
	MoveForwardAction(nullptr),
	MoveRightAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr)
{
}

void APossessMovePawnController::BeginPlay()
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