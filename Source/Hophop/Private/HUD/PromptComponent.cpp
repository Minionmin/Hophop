// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PromptComponent.h"
#include "HUD/RabbitLevelDisplayWidget.h"
#include "Components/TextBlock.h"
#include "Actors/Teleporter.h"

// ------------------------------ Teleporter ------------------------------
void UPromptComponent::SetTargetLevelText(FName TargetLevel)
{
	if (LevelDisplayWidget == nullptr)
	{
		LevelDisplayWidget = Cast<URabbitLevelDisplayWidget>(GetUserWidgetObject());
	}

	if (Teleporter == nullptr)
	{
		Teleporter = Cast<ATeleporter>(GetOwner());
	}

	if (LevelDisplayWidget)
	{
		LevelDisplayWidget->LevelLabel->SetText(FText::FromName(Teleporter->getTargetLevelName()));
	}
}
