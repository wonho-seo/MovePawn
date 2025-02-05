// Fill out your copyright notice in the Description page of Project Settings.


#include "DronePawn.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

#include "DroneController.h"

// Sets default values
ADronePawn::ADronePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleRootComponents"));
	RootBoxComponent->SetSimulatePhysics(false);
	SetRootComponent(RootBoxComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkeltalMesh"));
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->TargetArmLength = 800.0f;
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->SetupAttachment(RootComponent);

	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	ArrowComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();
}

void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator Move = FRotator::ZeroRotator - GetActorRotation();
	Move.Yaw = 0;
	if (!Move.IsNearlyZero())
	{
		AddActorLocalRotation(Move * DeltaTime * SpinSpeed * 0.01f);
	}
}

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ADroneController* PlayerController = Cast<ADroneController>(GetController()))
		{
			if (PlayerController->MoveForwardAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveForwardAction,
					ETriggerEvent::Triggered,
					this,
					&ADronePawn::MoveForward
				);
			}
			if (PlayerController->MoveRightAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveRightAction,
					ETriggerEvent::Triggered,
					this,
					&ADronePawn::MoveRight
				);
			}
			if (PlayerController->MoveUpAction)
			{

				EnhancedInput->BindAction(
					PlayerController->MoveUpAction,
					ETriggerEvent::Triggered,
					this,
					&ADronePawn::MoveUp
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ADronePawn::Look
				);
			}
			if (PlayerController->TurnRollAction)
			{
				EnhancedInput->BindAction(
					PlayerController->TurnRollAction,
					ETriggerEvent::Triggered,
					this,
					&ADronePawn::TurnRoll
				);
			}
		}
	}
}


void ADronePawn::MoveForward(const FInputActionValue& Value)
{
	const float MoveForwardInput = Value.Get<float>();

	if (GetWorld())
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		if (!FMath::IsNearlyZero(MoveForwardInput))
		{
			FVector ActorForwardDirection = GetActorForwardVector();
			ActorForwardDirection.Z = 0.0f;
			AddActorLocalRotation(FRotator(-MoveForwardInput, 0, -GetActorRotation().Roll * 0.1f) * DeltaTime * MoveSpeed);
			FRotator ActorRotation = GetActorRotation();
			ActorRotation.Pitch = FMath::Clamp(ActorRotation.Pitch, -MaxPitch, MaxPitch);
			SetActorRotation(ActorRotation);

			if (-ActorRotation.Pitch * MoveForwardInput > 0)
			{
				AddActorWorldOffset(ActorForwardDirection * MoveForwardInput * MoveSpeed * DeltaTime);
			}

		}
	}

}
void ADronePawn::MoveRight(const FInputActionValue& Value)
{
	const float MoveRightInput = Value.Get<float>();

	if (GetWorld())
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		if (!FMath::IsNearlyZero(MoveRightInput))
		{
			FVector ActorRightDirection = GetActorRightVector();
			ActorRightDirection.Z = 0.0f;

			AddActorLocalRotation(FRotator(-GetActorRotation().Pitch * 0.1f, 0, MoveRightInput) * DeltaTime * MoveSpeed);
			FRotator ActorRotation = GetActorRotation();
			ActorRotation.Roll = FMath::Clamp(ActorRotation.Roll, -MaxRoll, MaxRoll);
			SetActorRotation(ActorRotation);

			if (ActorRotation.Roll * MoveRightInput > 0)
			{
				AddActorWorldOffset(ActorRightDirection * MoveRightInput * MoveSpeed * DeltaTime);
			}
		}
	}

}

void ADronePawn::MoveUp(const FInputActionValue& Value)
{
	if (GetWorld())
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		const float MoveRightInput = Value.Get<float>();
		if (!FMath::IsNearlyZero(MoveRightInput))
		{
			AddActorLocalOffset(FVector(0.0f, 0.0f, 1.0f) * MoveRightInput * DeltaTime * MoveSpeed);
		}
	}
}

void ADronePawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();

	if (GetWorld())
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		if (!FMath::IsNearlyZero(LookInput.X))
		{
			AddActorLocalRotation(FRotator(0, LookInput.X, 0) * DeltaTime * SpinSpeed);
		}

		if (!FMath::IsNearlyZero(LookInput.Y))
		{
			AddActorLocalRotation(FRotator(LookInput.Y, 0, 0) * DeltaTime * SpinSpeed);

			FRotator ActorRotation = GetActorRotation();
			ActorRotation.Pitch = FMath::Clamp(ActorRotation.Pitch, -MaxPitch, MaxPitch);
			SetActorRotation(ActorRotation);
		}
	}
}

void ADronePawn::TurnRoll(const FInputActionValue& Value)
{
	const float LookInput = Value.Get<float>();

	if (GetWorld())
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();

		if (!FMath::IsNearlyZero(LookInput))
		{
			AddActorLocalRotation(FRotator(0, 0, LookInput) * DeltaTime * SpinSpeed);

			FRotator ActorRotation = GetActorRotation();
			ActorRotation.Pitch = FMath::Clamp(ActorRotation.Pitch, -MaxPitch, MaxPitch);
			SetActorRotation(ActorRotation);
		}
	}
}


