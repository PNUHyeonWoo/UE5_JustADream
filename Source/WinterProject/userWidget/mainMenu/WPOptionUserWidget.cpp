// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/mainMenu/WPOptionUserWidget.h"
#include <Components/Button.h>
#include "GameFramework/GameUserSettings.h"
#include "Components/Slider.h"
#include "savegame/WPOptionSaveGame.h"
#include <Kismet/GameplayStatics.h>

UWPOptionUserWidget::UWPOptionUserWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UWPOptionUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Cast<UButton>(GetWidgetFromName("EXITButton"))->OnClicked.AddDynamic(this, &UWPOptionUserWidget::OnClickEXITButton);

	for (int i = 0; i < 3; i++)
	{
		OptionWidgets.Add(GetWidgetFromName(*(FString(TEXT("OptionWidget")) + FString::FromInt(i))));
	}

	Cast<UButton>(GetWidgetFromName("OptionButton0"))->OnClicked.AddDynamic(this, &UWPOptionUserWidget::OnClickOptionButton0);
	Cast<UButton>(GetWidgetFromName("OptionButton1"))->OnClicked.AddDynamic(this, &UWPOptionUserWidget::OnClickOptionButton1);
	Cast<UButton>(GetWidgetFromName("OptionButton2"))->OnClicked.AddDynamic(this, &UWPOptionUserWidget::OnClickOptionButton2);

	Cast<UButton>(GetWidgetFromName("AutoSettingButton"))->OnClicked.AddDynamic(this, &UWPOptionUserWidget::OnClickAutoSettingButton);
	InitGraphicSettings();


	SetVisibility(ESlateVisibility::Hidden);

	OptionSaveGame = Cast<UWPOptionSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Option"), 0));

	if (!OptionSaveGame)
	{
		OptionSaveGame = UWPOptionSaveGame::CreateNewOptionSaveGame();
		UGameplayStatics::SaveGameToSlot(OptionSaveGame, TEXT("Option"), 0);
	}

	InitSoundSettings();

	Cast<USlider>(GetWidgetFromName("MusicSlider"))->OnValueChanged.AddDynamic(this, &UWPOptionUserWidget::OnChangeMusicSlider);
	Cast<USlider>(GetWidgetFromName("SFXSlider"))->OnValueChanged.AddDynamic(this, &UWPOptionUserWidget::OnChangeSFXSlider);

	FString LevelName = TEXT("");
	GetWorld()->GetCurrentLevel()->GetOuter()->GetName(LevelName);

	if (LevelName != FString(TEXT("MainMenuLevel")))
		GetWidgetFromName("OptionButton2")->SetVisibility(ESlateVisibility::Hidden);

}

void UWPOptionUserWidget::InitGraphicSettings()
{
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	Cast<USlider>(GetWidgetFromName("AntiAliasingSlider"))->SetValue(GameUserSettings->GetAntiAliasingQuality());
	Cast<USlider>(GetWidgetFromName("PostProcessingSlider"))->SetValue(GameUserSettings->GetPostProcessingQuality());
	Cast<USlider>(GetWidgetFromName("ShadowsSlider"))->SetValue(GameUserSettings->GetShadowQuality());
	Cast<USlider>(GetWidgetFromName("GlobalIllumiantionSlider"))->SetValue(GameUserSettings->GetGlobalIlluminationQuality());
	Cast<USlider>(GetWidgetFromName("ReflectionsSlider"))->SetValue(GameUserSettings->GetReflectionQuality());
	Cast<USlider>(GetWidgetFromName("TexturesSlider"))->SetValue(GameUserSettings->GetTextureQuality());
	Cast<USlider>(GetWidgetFromName("EffectsSlider"))->SetValue(GameUserSettings->GetVisualEffectQuality());
	Cast<USlider>(GetWidgetFromName("FoliageSlider"))->SetValue(GameUserSettings->GetFoliageQuality());
	Cast<USlider>(GetWidgetFromName("ShadingSlider"))->SetValue(GameUserSettings->GetShadingQuality());
}

void UWPOptionUserWidget::SaveGraphicSettngs()
{
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();

	GameUserSettings->SetAntiAliasingQuality(Cast<USlider>(GetWidgetFromName("AntiAliasingSlider"))->GetValue());
	GameUserSettings->SetPostProcessingQuality(Cast<USlider>(GetWidgetFromName("PostProcessingSlider"))->GetValue());
	GameUserSettings->SetShadowQuality(Cast<USlider>(GetWidgetFromName("ShadowsSlider"))->GetValue());
	GameUserSettings->SetGlobalIlluminationQuality(Cast<USlider>(GetWidgetFromName("GlobalIllumiantionSlider"))->GetValue());
	GameUserSettings->SetReflectionQuality(Cast<USlider>(GetWidgetFromName("ReflectionsSlider"))->GetValue());
	GameUserSettings->SetTextureQuality(Cast<USlider>(GetWidgetFromName("TexturesSlider"))->GetValue());
	GameUserSettings->SetVisualEffectQuality(Cast<USlider>(GetWidgetFromName("EffectsSlider"))->GetValue());
	GameUserSettings->SetFoliageQuality(Cast<USlider>(GetWidgetFromName("FoliageSlider"))->GetValue());
	GameUserSettings->SetShadingQuality(Cast<USlider>(GetWidgetFromName("ShadingSlider"))->GetValue());

	GameUserSettings->ApplySettings(false);
}

void UWPOptionUserWidget::InitSoundSettings()
{
	Cast<USlider>(GetWidgetFromName("MusicSlider"))->SetValue(OptionSaveGame->MusicVolume);
	Cast<USlider>(GetWidgetFromName("SFXSlider"))->SetValue(OptionSaveGame->SFXVolume);

	SetMusicVolume(OptionSaveGame->MusicVolume);
	SetSFXVolume(OptionSaveGame->SFXVolume);
}

void UWPOptionUserWidget::SaveSoundSettings()
{
	UGameplayStatics::SaveGameToSlot(OptionSaveGame, TEXT("Option"), 0);
}

void UWPOptionUserWidget::OnClickEXITButton()
{
	SaveGraphicSettngs();
	SaveSoundSettings();
	SetVisibility(ESlateVisibility::Hidden);
}

void UWPOptionUserWidget::OnClickOptionButton0()
{
	OnClickOptionButtonN(0);
}

void UWPOptionUserWidget::OnClickOptionButton1()
{
	OnClickOptionButtonN(1);
}

void UWPOptionUserWidget::OnClickOptionButton2()
{
	OnClickOptionButtonN(2);
}

void UWPOptionUserWidget::OnClickOptionButtonN(uint8 N)
{
	for (uint8 i = 0; i < OptionWidgets.Num(); i++)
		if (i == N)
			OptionWidgets[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		else
			OptionWidgets[i]->SetVisibility(ESlateVisibility::Hidden);
}

void UWPOptionUserWidget::OnClickAutoSettingButton()
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


	InitGraphicSettings();
}

void UWPOptionUserWidget::OnChangeMusicSlider(float Value)
{
	OptionSaveGame->MusicVolume = Value;
	SetMusicVolume(OptionSaveGame->MusicVolume);
}

void UWPOptionUserWidget::OnChangeSFXSlider(float Value)
{
	OptionSaveGame->SFXVolume = Value;
	SetSFXVolume(OptionSaveGame->SFXVolume);
}

void UWPOptionUserWidget::SetMusicVolume_Implementation(float Value)
{
}

void UWPOptionUserWidget::SetSFXVolume_Implementation(float Value)
{
}
