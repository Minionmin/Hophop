// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
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
class URabbitUserWidget;
class UCurveFloat;
class UCurveVector;
class UMotionWarpingComponent;

UCLASS()
class HOPHOP_API ARabbit : public ACharacter
{
	GENERATED_BODY()

public:

	ARabbit();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	UFUNCTION()
	void EnableUI();
	UFUNCTION()
	void DisableUI();
	
	bool ESCAlreadyPressed;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		class UGuideUIComponent* GuideUIComponent;

protected:

	virtual void BeginPlay() override;

private:
	
	// --------------Initialization--------------
	void Initialize();

	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* RabbitCapsuleComponent;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* RabbitMesh;
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* Boomstick;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;
	UPROPERTY(VisibleAnywhere)
		UMotionWarpingComponent* MotionWarpingComponent;

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
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* ESCAction;
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* ClimbAction;

	void Move(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void CancelRun(const FInputActionValue& Value);
	void ChargeJump(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void ESC(const FInputActionValue& Value);
	void Climb(const FInputActionValue& Value);

	UFUNCTION()
		void Climbing(FVector WorldDirection, float ScaleValue);
	UFUNCTION()
		void CalculateLedgeToMount(float TraceDistance, ECollisionChannel CollisionChannel);
	UFUNCTION()
		void StopClimbing();
	UFUNCTION()
		FHitResult BeginLineTrace(ECollisionChannel TraceChannel, FVector StartingPoint, FVector EndingPoint, FColor DebugColor);

	// ----------Variable----------
	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<ACarrotProjectile> CarrotProjectile;
	UPROPERTY(EditAnywhere, Category = Input)
		APlayerController* RabbitController;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<URabbitUserWidget> MainMenuWidget;
	UPROPERTY(VisibleAnywhere, Category = UI)
		class URabbitMainMenuWidget* MainMenuWidgetRef;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<URabbitUserWidget> MainMenuButtonWidget;
	UPROPERTY(VisibleAnywhere, Category = UI)
		class URabbitMainMenuButtonWidget* MainMenuButtonWidgetRef;
	bool isInAnimationMontage = false;

	// --------Timeline--------
	 // --------Climbing--------
	UFUNCTION()
		void ClimbRotationTimelineProgress(float Value);

	FOnTimelineFloat TimelineProgress;
	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* CurveFloat;
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
		FRotator StartClimbRotation;
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
		FRotator EndClimbRotation;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* MountLedgeMontage;

	// -------------Player Movement--------------
	UCharacterMovementComponent* RabbitMovementComponent;

	float WalkSpeed;
	UPROPERTY(EditAnywhere, Category = Initialization)
		float JumpMultiplier = 1.f;
	float OriginalJumpHeight;
	float PresentJumpHeight;
	float MaxJumpHeight;

	// For avoiding magic number
	float MaxRabbitFlySpeed = 200.f;
	float BrakingDecelerationFlyingSpeed = 3000.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MotionWarpHeight = 200.f;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		float ClimbLeftAndRight;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		float ClimbUpAndDown;

// Getter Setter
public:

	UFUNCTION()
		float GetClimbLeftAndRightValue() { return ClimbLeftAndRight; }
	UFUNCTION()
		float GetClimbUpAndDownValue() { return ClimbUpAndDown; }

};
