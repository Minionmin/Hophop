// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/HophopPlatform.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class HOPHOP_API AMovingPlatform : public AHophopPlatform
{
	GENERATED_BODY()

public:

	AMovingPlatform();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
		float MaxDistance = 100;

	UPROPERTY(EditAnywhere)
		FVector	PlatformVelocity = FVector(100, 0, 0);

	UPROPERTY(VisibleAnywhere)
		FVector OriginalLocation;

};
