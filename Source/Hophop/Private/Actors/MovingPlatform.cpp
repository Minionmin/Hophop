// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MovingPlatform.h"


AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;


}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetActorLocation();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation = CurrentLocation + PlatformVelocity * DeltaTime;
	SetActorLocation(CurrentLocation);
	float DistanceMoved = FVector::Dist(OriginalLocation, CurrentLocation);
	if (DistanceMoved > MaxDistance)
	{
		FVector MoveDirection = PlatformVelocity.GetSafeNormal();
		OriginalLocation = OriginalLocation + MoveDirection * MaxDistance;
		SetActorLocation(OriginalLocation);
		PlatformVelocity = -PlatformVelocity;
	}
}