// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/RabbitUserWidget.h"
#include "RabbitMainMenuButtonWidget.generated.h"

UENUM(BlueprintType)
enum class RabbitLevel : uint8
{
	MainMenu = 0 UMETA(DisplayName = "MainMenu"),
	IntroLevel UMETA(DisplayName = "IntroLevel"),
	Level1 UMETA(DisplayName = "Level1_1"),
	Level1A UMETA(DisplayName = "Level1A"),
	Level1B UMETA(DisplayName = "Level1B"),
	Level1C UMETA(DisplayName = "Level1C"),
	Level1D UMETA(DisplayName = "Level1D")
};

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
