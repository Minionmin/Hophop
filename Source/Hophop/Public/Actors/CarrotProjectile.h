// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarrotProjectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS()
class HOPHOP_API ACarrotProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	ACarrotProjectile();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UCapsuleComponent* Capsule;

};
