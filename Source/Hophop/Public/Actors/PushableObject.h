// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/InteractableInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PushableObject.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class HOPHOP_API APushableObject : public AActor, public IInteractableInterface
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

	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UBoxComponent* Box;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FVector Direction;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float Distance;

};
