// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClimbableObject.generated.h"

class UBoxComponent;

UCLASS()
class HOPHOP_API AClimbableObject : public AActor
{
	GENERATED_BODY()
	
public:	

	AClimbableObject();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:	

	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UBoxComponent* Box;

};
