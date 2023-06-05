// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HophopPlatform.generated.h"

UCLASS()
class HOPHOP_API AHophopPlatform : public AActor
{
	GENERATED_BODY()
	
public:	

	AHophopPlatform();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Initialization)
		UStaticMeshComponent* PlatformMesh;

	UPROPERTY(VisibleAnywhere, Category = Initialization)
		class UBoxComponent* Box;
};
