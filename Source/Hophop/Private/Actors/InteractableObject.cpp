// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/InteractableObject.h"
#include "Components/BoxComponent.h"
#include "HUD/PromptComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Rabbit.h"

AInteractableObject::AInteractableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	SetRootComponent(Box);

	WidgetCollision = CreateDefaultSubobject<UBoxComponent>("WidgetCollision");
	WidgetCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WidgetCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WidgetCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WidgetCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	WidgetCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	WidgetCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WidgetCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	WidgetCollision->SetupAttachment(GetRootComponent());

	PromptDisplay = CreateDefaultSubobject<UPromptComponent>("PromptDisplay");
	PromptDisplay->SetupAttachment(GetRootComponent());
	PromptDisplay->SetVisibility(false);
}

void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (!RabbitPlayer)
	{
		RabbitPlayer = Cast<ARabbit>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	}

	PromptDisplay->SetVisibility(false);
	WidgetCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObject::OnWidgetBoxOverlap);
	WidgetCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractableObject::OnWidgetBoxEndOverlap);
}

void AInteractableObject::OnWidgetBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PromptDisplay->SetVisibility(true);
}

void AInteractableObject::OnWidgetBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PromptDisplay->SetVisibility(false);
}

void AInteractableObject::ShowInRangePlayerUI()
{
	if (WidgetCollision->IsOverlappingActor(Cast<AActor>(RabbitPlayer)))
	{
		PromptDisplay->SetVisibility(true);
	}
	else
	{
		PromptDisplay->SetVisibility(false);
	}
}


void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableObject::IsInteracted_Implementation(ACharacter* InCharacter)
{
}