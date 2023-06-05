// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Rabbit.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ACarrotProjectile;

UCLASS()
class HOPHOP_API ARabbit : public ACharacter
{
	GENERATED_BODY()

public:

	ARabbit();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

protected:

	virtual void BeginPlay() override;

private:

	// --------------Initialization--------------
	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* RabbitMesh;
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* Boomstick;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	// --------------Input--------------
	UPROPERTY(EditAnywhere, Category = Input)
		UInputMappingContext* RabbitContext;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* RunAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* AttackAction;

	void Move(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void CancelRun(const FInputActionValue& Value);
	void ChargeJump(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

	// ----------Variable----------
	UPROPERTY(EditAnywhere, Category = Input)
		TSubclassOf<ACarrotProjectile> CarrotProjectile;

		// Player Movement

	float WalkSpeed;
	UPROPERTY(EditAnywhere, Category = Initialization)
	float JumpMultiplier = 1.f;
	float OriginalJumpHeight;
	float PresentJumpHeight;
	float MaxJumpHeight;
};
