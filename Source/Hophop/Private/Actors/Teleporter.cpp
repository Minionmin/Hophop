// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Teleporter.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/PromptComponent.h"
#include "NiagaraComponent.h"

ATeleporter::ATeleporter()
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	PromptDisplay->SetVisibility(false);
	PromptDisplay->SetTargetLevelText(getTargetLevelName());
	
}

void ATeleporter::IsInteracted_Implementation(ACharacter* InCharacter)
{
	if (!getTargetLevelName().IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), getTargetLevelName());
	}
}
