// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/InteractableInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PushableObject.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UPromptComponent;

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

	UFUNCTION(BlueprintCallable)
		void ShowInRangePlayerUI();

	UFUNCTION()
		void OnWidgetBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnWidgetBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Initialization)
		UBoxComponent* WidgetCollision;
	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UPromptComponent* PromptDisplay;
	UPROPERTY(VisibleAnywhere, Category = Initialization)
		class ARabbit* RabbitPlayer;

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FVector Direction;
	UPROPERTY(VisibleAnywhere)
		float Distance;

};
