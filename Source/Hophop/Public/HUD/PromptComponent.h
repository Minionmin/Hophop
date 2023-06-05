// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PromptComponent.generated.h"

/**
 * 
 */
UCLASS()
class HOPHOP_API UPromptComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	void SetTargetLevelText(FName TargetLevel);

protected:


private:

	class URabbitLevelDisplayWidget* LevelDisplayWidget;
	class ATeleporter* Teleporter;
};
