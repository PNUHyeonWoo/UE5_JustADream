// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/mainMenu/WPMainMenuUserWidget.h"
#include <Components/Button.h>
#include <WPGameInstance.h>
#include <saveGame/WPPlayerSaveGame.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/AssetManager.h>
#include <dataAsset/WPGunDataAsset.h>
#include <dataAsset/WPAmmoDataAsset.h>
#include <dataAsset/item/WPItemDataAsset.h>

void UWPMainMenuUserWidget::NativeConstruct()
{
	Cast<UButton>(GetWidgetFromName("PlayButton"))->OnClicked.AddDynamic(this, &UWPMainMenuUserWidget::OnClickPlayButton);

	Cast<UButton>(GetWidgetFromName("TutorialButton"))->OnClicked.AddDynamic(this, &UWPMainMenuUserWidget::OnClickTutorialButton);

	Cast<UButton>(GetWidgetFromName("ChallengeButton"))->OnClicked.AddDynamic(this, &UWPMainMenuUserWidget::OnClickChallengeButton);

	Cast<UButton>(GetWidgetFromName("SkillButton"))->OnClicked.AddDynamic(this, &UWPMainMenuUserWidget::OnClickSkillButton);

	Cast<UButton>(GetWidgetFromName("OptionButton"))->OnClicked.AddDynamic(this, &UWPMainMenuUserWidget::OnClickOptionButton);
	
}

void UWPMainMenuUserWidget::OnClickPlayButton()
{
	if (!CanPlayGame())
		return;

	UWPPlayerSaveGame* PlayerSaveGame = UWPPlayerSaveGame::CreateNewPlayerSaveGame();

	UGameplayStatics::SaveGameToSlot(PlayerSaveGame, Cast<UWPGameInstance>(GetGameInstance())->PlayerSlot, 0);

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Default"));

}

void UWPMainMenuUserWidget::OnClickTutorialButton()
{
	if (!CanPlayGame())
		return;

	UWPPlayerSaveGame* PlayerSaveGame = UWPPlayerSaveGame::CreateNewPlayerSaveGame();

	UGameplayStatics::SaveGameToSlot(PlayerSaveGame, Cast<UWPGameInstance>(GetGameInstance())->PlayerSlot, 0);

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TutorialLevel"));

}

void UWPMainMenuUserWidget::OnClickChallengeButton()
{
	GetWidgetFromName("ChallengeWidget")->SetVisibility(ESlateVisibility::Visible);
}

void UWPMainMenuUserWidget::OnClickSkillButton()
{
	GetWidgetFromName("SkillWidget")->SetVisibility(ESlateVisibility::Visible);
}

void UWPMainMenuUserWidget::OnClickOptionButton()
{
	GetWidgetFromName("OptionWidget")->SetVisibility(ESlateVisibility::Visible);
}

void UWPMainMenuUserWidget::GetExperience1000()
{
	UWPGameInstance* GameInstance = Cast<UWPGameInstance>(GetGameInstance());
	GameInstance->SetExperience(GameInstance->GetExperience() + 1000);
	GameInstance->SaveSkill();
}

bool UWPMainMenuUserWidget::CanPlayGame()
{
	return GetWidgetFromName("ChallengeWidget")->GetVisibility() == ESlateVisibility::Hidden && GetWidgetFromName("SkillWidget")->GetVisibility() == ESlateVisibility::Hidden && GetWidgetFromName("OptionWidget")->GetVisibility() == ESlateVisibility::Hidden;

}
