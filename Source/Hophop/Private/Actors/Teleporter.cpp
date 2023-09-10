// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Teleporter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/PromptComponent.h"
#include "NiagaraComponent.h"

ATeleporter::ATeleporter()
{
	Box->CanCharacterStepUpOn = ECB_No;
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	Portal = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Portal"));
	Portal->SetupAttachment(GetRootComponent());

}

void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleporter::BeginPlay()
{
	Super::BeginPlay();
	PromptDisplay->SetTargetLevelText(getTargetLevelName());
	
}

void ATeleporter::IsInteracted_Implementation(ACharacter* InCharacter)
{
	if (!getTargetLevelName().IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), getTargetLevelName());
	}
}
