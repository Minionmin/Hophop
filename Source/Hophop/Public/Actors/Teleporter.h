// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PushableObject.h"
#include "Teleporter.generated.h"

/**
 * 
 */
UCLASS()
class HOPHOP_API ATeleporter : public AInteractableObject
{
	GENERATED_BODY()
	
public:

	ATeleporter();
	virtual void Tick(float DeltaTime) override;

	// ----------Interface----------
	virtual void IsInteracted_Implementation(ACharacter* InCharacter) override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "TargetLevel")
		FName TargetLevel;
	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* Portal;

public:

	FName getTargetLevelName() { return TargetLevel; }
};
