// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RabbitMainMenuButtonWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Player/Rabbit.h"

void URabbitMainMenuButtonWidget::StartResumeText()
{
	CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (CurrentLevelName != "MainMenu")
	{
		StartGameLabel->SetText(FText::FromString("Resume"));
	}
	else
	{
		StartGameLabel->SetText(FText::FromString("StartGame"));
	}
}

void URabbitMainMenuButtonWidget::StartClicked()
{
	FString TargetLevel = UEnum::GetDisplayValueAsText(RabbitLevel::IntroLevel).ToString();
	UGameplayStatics::OpenLevel(GetWorld(), FName(*TargetLevel));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
}

void URabbitMainMenuButtonWidget::ResumeClicked()
{
	RabbitPlayer->DisableUI();
	RabbitPlayer->ESCAlreadyPressed = false;
}

void URabbitMainMenuButtonWidget::StartResumeClick()
{
	CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (!RabbitPlayer)
	{
		RabbitPlayer = Cast<ARabbit>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	}

	if (CurrentLevelName != "MainMenu") // In game ("Resume")
	{
		if (RabbitPlayer)
		{
			ResumeClicked();
		}
	}
	else // In main menu ("Start Game")
	{
		StartClicked();
	}

}


