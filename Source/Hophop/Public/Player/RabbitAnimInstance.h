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
	
	UPROPERTY(BlueprintReadOnly, Category = Initialization)
		class ARabbit* Rabbit;
	UPROPERTY(BlueprintReadOnly, Category = Initialization)
		class UCharacterMovementComponent* RabbitMovement;
	UPROPERTY(BlueprintReadOnly, Category = State)
		float GroundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = State)
		bool IsFalling;
};
