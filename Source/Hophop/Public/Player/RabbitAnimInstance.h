// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RabbitAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOPHOP_API URabbitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	// Anim Notify
	UFUNCTION(BlueprintCallable)
		void GetGroundSurfaceType(bool& OutHasHit, FVector& OutPlayerLocation, TEnumAsByte<EPhysicalSurface>& OutSurface);

	UPROPERTY(BlueprintReadOnly, Category = "Initialization")
		class ARabbit* Rabbit;
	UPROPERTY(BlueprintReadOnly, Category = "Initialization")
		class UCharacterMovementComponent* RabbitMovementComponent;
	UPROPERTY(BlueprintReadOnly, Category = "State")
		float GroundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "State")
		bool IsFalling;
	UPROPERTY(BlueprintReadOnly, Category = "State")
		bool IsClimbing;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float ClimbLeftAndRight;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float ClimbUpAndDown;

};
