// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOPHOP_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = Interact)
		void IsInteracted(ACharacter* InCharacter);
};

// -BlueprintImplimentableEvent only generates code that call event on blueprint side, this event can be only be defined in blueprint
//
// -While ÅgBlueprintNativeEventÅh will declare extra special virtual function with _Implementation surrfix in generated code which you can override and 
// call it when event is called in C++ and blueprint as well as you can define this event in blueprint to.
//
// In short it a choose to either add C++ layer to the event or not.If you donÅft plan to define event in C++ use first one if not then use 2nd.
// If you use BlueprintNativeEvent you WILL NEED to define _Implementation function or else you gonna have linker errors, 
// it main reason why you need to choose between the 2 so you donÅft need to create empty definitions for all events which only gonna be used in blueprint anyway
