// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WPOptionSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPOptionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	static UWPOptionSaveGame* CreateNewOptionSaveGame();

public:
	UPROPERTY()
	float MusicVolume;
	UPROPERTY()
	float SFXVolume;
};
