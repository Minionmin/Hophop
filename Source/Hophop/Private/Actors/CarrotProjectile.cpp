// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/CarrotProjectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACarrotProjectile::ACarrotProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	SetRootComponent(Capsule);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Capsule);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

}

void ACarrotProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACarrotProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

