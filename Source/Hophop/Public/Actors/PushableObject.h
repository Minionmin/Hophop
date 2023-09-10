// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/InteractableObject.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PushableObject.generated.h"

UCLASS()
class HOPHOP_API APushableObject : public AInteractableObject
{
	GENERATED_BODY()
	
public:	

	APushableObject();
	virtual void Tick(float DeltaTime) override;

	// ----------Interface----------
	virtual void IsInteracted_Implementation(ACharacter* InCharacter) override;

	// ----------Function----------
	UFUNCTION(BlueprintCallable)
		void CalculateToGoDirection(ACharacter* InCharacter);

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FVector Direction;
	UPROPERTY(VisibleAnywhere)
		float Distance;

};
