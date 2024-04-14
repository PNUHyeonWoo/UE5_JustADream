// Fill out your copyright notice in the Description page of Project Settings.


#include "WPGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include "saveGame/WPChallengeSaveGame.h"
#include "saveGame/WPSkillSaveGame.h"

void UWPGameInstance::SetChallangeSaveGame(TObjectPtr<class UWPChallengeSaveGame> NewSaveGame)
{
	ChallangeSaveGame = NewSaveGame;
}

void UWPGameInstance::SetSkillSaveGame(TObjectPtr<class UWPSkillSaveGame> NewSaveGame)
{
	SkillSaveGame = NewSaveGame;
}

void UWPGameInstance::SaveChallenge()
{
	UGameplayStatics::SaveGameToSlot(ChallangeSaveGame, ChallangeSlot, 0);
}

void UWPGameInstance::SaveSkill()
{
	UGameplayStatics::SaveGameToSlot(SkillSaveGame, SkillSlot, 0);
}

uint8 UWPGameInstance::GetChallengeWeight(FName key)
{
	return ChallangeSaveGame->ChallengeWeights[key];
}

uint8 UWPGameInstance::GetSkillLevel(FName key)
{
	return SkillSaveGame->SkillLevels[key];
}

uint32 UWPGameInstance::GetExperience()
{
	return SkillSaveGame->Experience;
}

void UWPGameInstance::SetChallengeWeight(FName key, uint8 value)
{
	ChallangeSaveGame->ChallengeWeights[key] = value;
}

void UWPGameInstance::SetSkillLevel(FName key, uint8 value)
{
	SkillSaveGame->SkillLevels[key] = value;
}

void UWPGameInstance::SetExperience(uint32 value)
{
	SkillSaveGame->Experience = value;
}

void UWPGameInstance::LogData()
{
	UE_LOG(LogTemp, Log, TEXT("------------------------"));

	for (const TPair<FName, uint8>& pair : ChallangeSaveGame->ChallengeWeights)
	{
		UE_LOG(LogTemp, Log, TEXT("%s : %d"), *(pair.Key.ToString()), pair.Value);
	}

	UE_LOG(LogTemp, Log, TEXT("------------------------"));

	UE_LOG(LogTemp, Log, TEXT("Exp : %d"), SkillSaveGame->Experience);

	UE_LOG(LogTemp, Log, TEXT("------------------------"));

	for (const TPair<FName, uint8>& pair : SkillSaveGame->SkillLevels)
	{
		UE_LOG(LogTemp, Log, TEXT("%s : %d"), *(pair.Key.ToString()), pair.Value);
	}

	UE_LOG(LogTemp, Log, TEXT("------------------------"));
}

