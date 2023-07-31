// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/RabbitUserWidget.h"
#include "RabbitMainMenuButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOPHOP_API URabbitMainMenuButtonWidget : public URabbitUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		void StartResumeText();

	UFUNCTION(BlueprintCallable)
		void StartResumeClick();
	UFUNCTION()
		void StartClicked();
	UFUNCTION()
		void ResumeClicked();

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* StartGameLabel;

private:

	FString CurrentLevelName;
	class ARabbit* RabbitPlayer;

};
