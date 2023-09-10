// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/InteractableInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

class UBoxComponent;
class UPromptComponent;
class UStaticMeshComponent;


UCLASS()
class HOPHOP_API AInteractableObject : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	

	AInteractableObject();

	// ----------Interface----------
	virtual void IsInteracted_Implementation(ACharacter* InCharacter) override;
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		void OnWidgetBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnWidgetBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void ShowInRangePlayerUI();

	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Initialization)
		UBoxComponent* WidgetCollision;
	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UPromptComponent* PromptDisplay;
	UPROPERTY(VisibleAnywhere, Category = Initialization)
		class ARabbit* RabbitPlayer;

public:	

	virtual void Tick(float DeltaTime) override;

};
