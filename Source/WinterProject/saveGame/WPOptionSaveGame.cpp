// Fill out your copyright notice in the Description page of Project Settings.


#include "saveGame/WPOptionSaveGame.h"
#include <Kismet/GameplayStatics.h>

UWPOptionSaveGame* UWPOptionSaveGame::CreateNewOptionSaveGame()
{
	UWPOptionSaveGame* OptionSaveGame = Cast<UWPOptionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWPOptionSaveGame::StaticClass()));
	OptionSaveGame->MusicVolume = 1;
	OptionSaveGame->SFXVolume = 1;

	return OptionSaveGame;
}
