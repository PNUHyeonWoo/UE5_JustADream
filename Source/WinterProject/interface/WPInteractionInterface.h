// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WPInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWPInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WINTERPROJECT_API IWPInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(class AWPPlayerActor* PlayerActor) = 0;
};
