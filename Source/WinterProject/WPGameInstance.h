// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<class UWPChallengeSaveGame> ChallangeSaveGame;

	UPROPERTY()
	TObjectPtr<class UWPSkillSaveGame> SkillSaveGame;

	const FString ChallangeSlot = TEXT("Challange");
	const FString SkillSlot = TEXT("Skill");
	const FString PlayerSlot = TEXT("Player");

public:
	void SetChallangeSaveGame(TObjectPtr<class UWPChallengeSaveGame> NewSaveGame);
	void SetSkillSaveGame(TObjectPtr<class UWPSkillSaveGame> NewSaveGame);
	void SaveChallenge();
	void SaveSkill();

	uint8 GetChallengeWeight(FName key);
	uint8 GetSkillLevel(FName key);
	uint32 GetExperience();

	void SetChallengeWeight(FName key,uint8 value);
	void SetSkillLevel(FName key,uint8 value);
	void SetExperience(uint32 value);

	void LogData();

};
