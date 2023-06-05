// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/RabbitUserWidget.h"
#include "RabbitLevelDisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOPHOP_API URabbitLevelDisplayWidget : public URabbitUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LevelLabel;
};
