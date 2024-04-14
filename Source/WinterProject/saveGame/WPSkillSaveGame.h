// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WPSkillSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPSkillSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	void Integrity();

public:
	UPROPERTY()
	uint32 Experience;

	UPROPERTY()
	TMap<FName, uint8> SkillLevels;
	
};
