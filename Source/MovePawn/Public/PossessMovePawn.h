// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PossessMovePawn.generated.h"
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;
struct FInputActionValue;

UCLASS()
class MOVEPAWN_API APossessMovePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APossessMovePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float MoveAccelerationSpeed = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float MoveMaxSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float SpinSpeed = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float MinViewAngle = -60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float MaxViewAngle = 40.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float GravityValue = 9.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float DownSpeedRate = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float JumpSpeed = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float JumpMoveAccelerationRate = 0.3f;


	UFUNCTION()
	void MoveForward(const FInputActionValue& Value);
	UFUNCTION()
	void MoveRight(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Jump(const FInputActionValue& Value);
	UFUNCTION()
	void StopInput();
private:
	FVector MoveSpeedVector = FVector::ZeroVector;
	float DownSpeed = 0.0f;
	float JumpEpsilon = 1.0f;
	float MoveEpsilon = 1.0f;
	bool bIsJump = true;
	FCollisionShape CollisionShape;
};
