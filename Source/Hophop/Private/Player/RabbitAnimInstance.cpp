// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RabbitAnimInstance.h"
#include "Player/Rabbit.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

void URabbitAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Rabbit = Cast<ARabbit>(TryGetPawnOwner());
	if (Rabbit)
	{
		RabbitMovementComponent = Rabbit->GetCharacterMovement();
	}

}

void URabbitAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (RabbitMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(RabbitMovementComponent->Velocity);
		IsFalling = RabbitMovementComponent->IsFalling();
		IsClimbing = RabbitMovementComponent->MovementMode == EMovementMode::MOVE_Flying;

		// Climbing variables
		ClimbLeftAndRight = Rabbit->GetClimbLeftAndRightValue();
		ClimbUpAndDown = Rabbit->GetClimbUpAndDownValue();
		if (RabbitMovementComponent->Velocity.Size() == 0) ClimbLeftAndRight = ClimbUpAndDown = 0.f;
	}
}

// Making parameters reference-base will tell Blueprint that they are "out" variable
void URabbitAnimInstance::GetGroundSurfaceType(bool& OutHasHit, FVector& OutPlayerLocation, TEnumAsByte<EPhysicalSurface>& OutSurface)
{
	float TraceHeight = 150.f;

	// Parameters
	FHitResult HitSurfaceResult;
	FVector PlayerStartLocation = Rabbit->GetActorLocation();
	FVector PlayerEndLocation = Rabbit->GetActorLocation() - FVector(0.f, 0.f, TraceHeight);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Rabbit);
	Params.bReturnPhysicalMaterial = true;

	// Trace for floor type
	bool bHasHit = GetWorld()->LineTraceSingleByChannel(
		HitSurfaceResult,
		PlayerStartLocation,
		PlayerEndLocation,
		ECollisionChannel::ECC_Visibility,
		Params
	);

	OutHasHit = bHasHit;
	OutPlayerLocation = PlayerStartLocation;
	
	if (bHasHit)
	{
		if (HitSurfaceResult.PhysMaterial != nullptr)
		{
			OutSurface = HitSurfaceResult.PhysMaterial->SurfaceType;
		}
	}
}


