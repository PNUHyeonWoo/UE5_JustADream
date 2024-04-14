// Fill out your copyright notice in the Description page of Project Settings.


#include "gameMode/WPLoadLevelGameMode.h"
#include "saveGame/WPSkillSaveGame.h"
#include "saveGame/WPChallengeSaveGame.h"
#include "WPGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include <saveGame/WPPlayerSaveGame.h>
#include "GameFramework/GameUserSettings.h"


void AWPLoadLevelGameMode::BeginPlay()
{
	Super::BeginPlay();



	UWPGameInstance* GameInstance = Cast<UWPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	bool IsFirstPlay = false;

	UWPChallengeSaveGame* ChallangeSaveGame;
	ChallangeSaveGame = Cast<UWPChallengeSaveGame>(UGameplayStatics::LoadGameFromSlot(GameInstance->ChallangeSlot, 0));
	if (!ChallangeSaveGame)
	{
		ChallangeSaveGame = Cast<UWPChallengeSaveGame>(UGameplayStatics::CreateSaveGameObject(UWPChallengeSaveGame::StaticClass()));
		IsFirstPlay = true;
	}
	ChallangeSaveGame->Integrity();

	GameInstance->SetChallangeSaveGame(ChallangeSaveGame);
	GameInstance->SaveChallenge();



	UWPSkillSaveGame* SkillSaveGame;
	SkillSaveGame = Cast<UWPSkillSaveGame>(UGameplayStatics::LoadGameFromSlot(GameInstance->SkillSlot, 0));
	if (!SkillSaveGame)
	{
		SkillSaveGame = Cast<UWPSkillSaveGame>(UGameplayStatics::CreateSaveGameObject(UWPSkillSaveGame::StaticClass()));
		SkillSaveGame->Experience = 0;
	}
	SkillSaveGame->Integrity();

	GameInstance->SetSkillSaveGame(SkillSaveGame);
	GameInstance->SaveSkill();

	if(!IsFirstPlay)
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenuLevel"));
	else 
	{
		UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();

		GameUserSettings->RunHardwareBenchmark();
		GameUserSettings->ApplySettings(false);

		GameUserSettings->SetAntiAliasingQuality(GameUserSettings->GetAntiAliasingQuality() == 0 ? 0 : GameUserSettings->GetAntiAliasingQuality() - 1);
		GameUserSettings->SetPostProcessingQuality(GameUserSettings->GetPostProcessingQuality() == 0 ? 0 : GameUserSettings->GetPostProcessingQuality() - 1);
		GameUserSettings->SetShadowQuality(GameUserSettings->GetShadowQuality() == 0 ? 0 : GameUserSettings->GetShadowQuality() - 1);
		GameUserSettings->SetGlobalIlluminationQuality(GameUserSettings->GetGlobalIlluminationQuality() == 0 ? 0 : GameUserSettings->GetGlobalIlluminationQuality() - 1);
		GameUserSettings->SetReflectionQuality(GameUserSettings->GetReflectionQuality() == 0 ? 0 : GameUserSettings->GetReflectionQuality() - 1);
		GameUserSettings->SetTextureQuality(GameUserSettings->GetTextureQuality() == 0 ? 0 : GameUserSettings->GetTextureQuality() - 1);
		GameUserSettings->SetVisualEffectQuality(GameUserSettings->GetVisualEffectQuality() == 0 ? 0 : GameUserSettings->GetVisualEffectQuality() - 1);
		GameUserSettings->SetFoliageQuality(GameUserSettings->GetFoliageQuality() == 0 ? 0 : GameUserSettings->GetFoliageQuality() - 1);
		GameUserSettings->SetShadingQuality(GameUserSettings->GetShadingQuality() == 0 ? 0 : GameUserSettings->GetShadingQuality() - 1);

		GameUserSettings->ApplySettings(false);

		UWPPlayerSaveGame* PlayerSaveGame = UWPPlayerSaveGame::CreateNewPlayerSaveGame();
		UGameplayStatics::SaveGameToSlot(PlayerSaveGame, Cast<UWPGameInstance>(GetGameInstance())->PlayerSlot, 0);
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("TutorialLevel"));
	}
}