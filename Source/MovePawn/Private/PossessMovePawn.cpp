// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessMovePawn.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Components/ArrowComponent.h"

#include "PossessMovePawnController.h"

// Sets default values
APossessMovePawn::APossessMovePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleRootComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleRootComponents"));
	CapsuleRootComponent->SetSimulatePhysics(false);
	CapsuleRootComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	SetRootComponent(CapsuleRootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeltalMesh"));
	SkeletalMeshComponent->SetSimulatePhysics(false);
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->SetupAttachment(RootComponent);

	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	ArrowComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APossessMovePawn::BeginPlay()
{
	Super::BeginPlay();

	CollisionShape = FCollisionShape::MakeCapsule(CapsuleRootComponent->GetUnscaledCapsuleRadius(), CapsuleRootComponent->GetUnscaledCapsuleHalfHeight());
}

// Called every frame
void APossessMovePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DownSpeed -= GravityValue * DeltaTime;
	MoveSpeedVector.Z += DownSpeed * DeltaTime;

	auto Sign = [](float Value) -> float
		{
			return (Value > 0) ? 1.0f : -1.0f;
		};
	FVector SignVector(Sign(MoveSpeedVector.X), Sign(MoveSpeedVector.Y), Sign(MoveSpeedVector.Z));
	FVector FrontVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();

	FHitResult HitFrontResult;
	FHitResult HitSideResult;
	FHitResult HitJumpResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bFrontHit = GetWorld()->SweepSingleByChannel(
		HitFrontResult,
		GetActorLocation(),
		GetActorLocation() + FrontVector * MoveEpsilon * SignVector.X,
		FQuat::Identity,
		ECC_Visibility,
		CollisionShape,
		CollisionParams
	);
	bool bSideHit = GetWorld()->SweepSingleByChannel(
		HitSideResult,
		GetActorLocation(),
		GetActorLocation() + RightVector * MoveEpsilon * SignVector.Y,
		FQuat::Identity,
		ECC_Visibility,
		CollisionShape,
		CollisionParams
	);
	bool bJumpHit = GetWorld()->SweepSingleByChannel(
		HitJumpResult,
		GetActorLocation(),
		GetActorLocation() + FVector(0, 0, JumpEpsilon * SignVector.Z),
		FQuat::Identity,
		ECC_Visibility,
		CollisionShape,
		CollisionParams
	);

	if (bFrontHit)
	{
		MoveSpeedVector.X = 0;
	}
	if (bSideHit)
	{

		MoveSpeedVector.Y = 0;
	}
	if (bJumpHit)
	{

		if (bIsJump)
		{
			bIsJump = false;
		}
		MoveSpeedVector.Z = 0;

	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(FrontVector * MoveEpsilon * SignVector.X).ToString());

	AddActorLocalOffset(MoveSpeedVector);
}

// Called to bind functionality to input
void APossessMovePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APossessMovePawnController* PlayerController = Cast<APossessMovePawnController>(GetController()))
		{
			if (PlayerController->MoveForwardAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveForwardAction,
					ETriggerEvent::Triggered,
					this,
					&APossessMovePawn::MoveForward
				);
			}
			if (PlayerController->MoveForwardAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveForwardAction,
					ETriggerEvent::Completed,
					this,
					&APossessMovePawn::StopInput
				);
			}
			if (PlayerController->MoveRightAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveRightAction,
					ETriggerEvent::Triggered,
					this,
					&APossessMovePawn::MoveRight
				);
			}
			if (PlayerController->MoveRightAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveRightAction,
					ETriggerEvent::Completed,
					this,
					&APossessMovePawn::StopInput
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&APossessMovePawn::Look
				);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&APossessMovePawn::Jump
				);
			}
		}
	}

}

void APossessMovePawn::MoveForward(const FInputActionValue& Value)
{
	const float MoveForwardInput = Value.Get<float>();
	if (GetWorld())
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		if (!FMath::IsNearlyZero(MoveForwardInput))
		{
			float Move = MoveForwardInput * MoveAccelerationSpeed * DeltaTime;
			if (bIsJump)
			{
				Move *= JumpMoveAccelerationRate;
			}
			
			MoveSpeedVector.X += MoveSpeedVector.X < 10 * Move ? 10 * Move : Move;
			MoveSpeedVector = MoveSpeedVector.GetClampedToMaxSize(MoveMaxSpeed);
		}
	}
}

void APossessMovePawn::MoveRight(const FInputActionValue& Value)
{
	const float MoveRightInput = Value.Get<float>();
	if (GetWorld())
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		if (!FMath::IsNearlyZero(MoveRightInput))
		{
			float Move = MoveRightInput * MoveAccelerationSpeed * DeltaTime;
			if (bIsJump)
			{
				Move *= JumpMoveAccelerationRate;
			}
			MoveSpeedVector.Y += Move;
			MoveSpeedVector = MoveSpeedVector.GetClampedToMaxSize(MoveMaxSpeed);
			//AddActorLocalOffset(MoveSpeedVector);
		}
	}
}

void APossessMovePawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float Spin = SpinSpeed * DeltaTime;
	if (!FMath::IsNearlyZero(LookInput.X))
	{
		AddActorLocalRotation(FRotator(0, LookInput.X, 0) * Spin);
	}

	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		SpringArmComponent->AddRelativeRotation(FRotator(LookInput.Y, 0, 0) * Spin);

		FRotator Rotator = SpringArmComponent->GetRelativeRotation();
		Rotator.Pitch = FMath::Clamp(Rotator.Pitch, MinViewAngle, MaxViewAngle);
		SpringArmComponent->SetRelativeRotation(Rotator);
	}
}

void APossessMovePawn::Jump(const FInputActionValue& Value)
{
	if (!bIsJump)
	{
		AddActorLocalOffset(FVector(0.0f, 0.0f, 2 * JumpEpsilon));

		bIsJump = true;
		DownSpeed = JumpSpeed;
	}
}

void APossessMovePawn::StopInput()
{
	if (!bIsJump)
	{
		MoveSpeedVector.X = 0;
		MoveSpeedVector.Y = 0;
	}
}

