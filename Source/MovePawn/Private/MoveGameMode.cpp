// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveGameMode.h"

#include "PossessMovePawn.h"
#include "PossessMovePawnController.h"
#include "DronePawn.h"
#include "DroneController.h"

AMoveGameMode::AMoveGameMode()
{
	//DefaultPawnClass = APossessMovePawn::StaticClass();
	//PlayerControllerClass = APossessMovePawnController::StaticClass();
	DefaultPawnClass = ADronePawn::StaticClass();
	PlayerControllerClass = ADroneController::StaticClass();
}
