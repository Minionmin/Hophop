// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RabbitAnimInstance.h"
#include "Player/Rabbit.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URabbitAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Rabbit = Cast<ARabbit>(TryGetPawnOwner());
	if (Rabbit)
	{
		RabbitMovement = Rabbit->GetCharacterMovement();
	}

}

void URabbitAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (RabbitMovement)
	{
		GroudSpeed = UKismetMathLibrary::VSizeXY(RabbitMovement->Velocity);
	}
}
