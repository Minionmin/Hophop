// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/ClimbableObject.h"
#include "Components/BoxComponent.h"


AClimbableObject::AClimbableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Block);
	SetRootComponent(Box);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Box);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->CanCharacterStepUpOn = ECB_No;
	
}

void AClimbableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AClimbableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

