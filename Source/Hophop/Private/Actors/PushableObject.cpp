// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PushableObject.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

APushableObject::APushableObject()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	SetRootComponent(Box);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Mesh->SetupAttachment(GetRootComponent());


}

void APushableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void APushableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APushableObject::IsInteracted_Implementation(ACharacter* InCharacter)
{
}

void APushableObject::CalculateToGoDirection(ACharacter* InCharacter)
{
	FVector Forward = GetActorForwardVector();
	FVector CharacterLocation = InCharacter->GetActorLocation();

	// Adjustcharacter Z so they are the same
	FVector AdjustedLocation = FVector(CharacterLocation.X, CharacterLocation.Y, GetActorLocation().Z);
	FVector ToCharacter = (AdjustedLocation - GetActorLocation()).GetSafeNormal();

	// Get angle by dotting vectors
	const float CosTheta = FVector::DotProduct(Forward, ToCharacter);
	float Radian = FMath::Acos(CosTheta);
	float Degree = FMath::RadiansToDegrees(Radian);

	// CrossProduct to check if the cross vector pointing up or down
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToCharacter);
	if (CrossProduct.Z < 0)
	{
		Degree *= -1.f;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Degree: %f"), Degree);

	if (Degree >= -45.f && Degree < 45.f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Forward"));
		Direction = GetActorForwardVector() * -Distance;
	}
	else if (Degree >= 45.f && Degree < 135.f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Right"));
		Direction = GetActorRightVector() * -Distance;
	}
	else if (Degree >= -135.f && Degree < -45.f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Left"));
		Direction = GetActorRightVector() * Distance;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Back"));
		Direction = GetActorForwardVector() * Distance;
	}

	// Final result
	Direction = GetActorLocation() + Direction;
}



