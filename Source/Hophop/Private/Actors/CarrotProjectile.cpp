// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/CarrotProjectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACarrotProjectile::ACarrotProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Capsule->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
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

