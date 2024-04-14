// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WPChallengeSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPChallengeSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	void Integrity();
	
public:
	UPROPERTY()
	TMap<FName, uint8> ChallengeWeights;

};
