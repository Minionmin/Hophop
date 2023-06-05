// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Teleporter.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/PromptComponent.h"

ATeleporter::ATeleporter()
{
}

void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleporter::BeginPlay()
{
	Super::BeginPlay();
	PromptDisplay->SetTargetLevelText(TargetLevel);
	
}

void ATeleporter::IsInteracted_Implementation(ACharacter* InCharacter)
{
	if (!TargetLevel.IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), TargetLevel);
	}
}
