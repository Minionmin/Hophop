// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HophopPlatform.h"
#include "Components/BoxComponent.h"

AHophopPlatform::AHophopPlatform()
{

	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	//Box->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Box->SetBoxExtent(FVector(128.f,64.f,5.f), true);
	SetRootComponent(Box);

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlatformMesh");
	PlatformMesh->SetupAttachment(Box);
	PlatformMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AHophopPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHophopPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

