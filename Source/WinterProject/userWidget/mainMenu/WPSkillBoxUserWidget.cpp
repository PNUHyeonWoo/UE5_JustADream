// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/mainMenu/WPSkillBoxUserWidget.h"
#include <Engine/AssetManager.h>
#include <dataAsset/WPSkillDataAsset.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <WPGameInstance.h>
#include <Components/Button.h>

void UWPSkillBoxUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UAssetManager& Manager = UAssetManager::Get();

    FString RenderName(TEXT(""));

    FString NameString = GetFName().ToString();

    RenderName.AppendChar(NameString[0]);

    for (int i = 1; i < NameString.Len(); i++)
    {
        if (isupper(NameString[i]) && i < NameString.Len() - 1)
            RenderName += TEXT(" ");

        RenderName.AppendChar(NameString[i]);
    }

    Cast<UTextBlock>(GetWidgetFromName("SkillName"))->SetText(FText::FromString(RenderName));

    FAssetData AssetData;
    Manager.GetPrimaryAssetData(FPrimaryAssetId("WPSkillDataAsset", GetFName()), AssetData);

    SkillDataAsset = Cast<UWPSkillDataAsset>(AssetData.GetAsset());

    Cast<UImage>(GetWidgetFromName("Image"))->SetBrushFromTexture(SkillDataAsset->SkillIcon);

    uint8 MaxLevel = SkillDataAsset->NeedExpForLevelUP.Num();

    Cast<UTextBlock>(GetWidgetFromName("MaxLevel"))->SetText(FText::FromString(FString::FromInt(MaxLevel)));

    uint8 CurrentLevel = Cast<UWPGameInstance>(GetGameInstance())->GetSkillLevel(GetFName());

    Cast<UTextBlock>(GetWidgetFromName("CurrentLevel"))->SetText(FText::FromString(FString::FromInt(CurrentLevel)));

    if(CurrentLevel < MaxLevel)
        Cast<UTextBlock>(GetWidgetFromName("Cost"))->SetText(FText::FromString(FString::FromInt(SkillDataAsset->NeedExpForLevelUP[CurrentLevel])));
    else
        Cast<UTextBlock>(GetWidgetFromName("Cost"))->SetText(FText::FromString(TEXT("MAX")));

    Cast<UButton>(GetWidgetFromName("LevelUpButton"))->OnClicked.AddDynamic(this, &UWPSkillBoxUserWidget::OnClickLevelUpButton);

}

void UWPSkillBoxUserWidget::OnClickLevelUpButton()
{
    uint32 CurrentExperience = Cast<UWPGameInstance>(GetGameInstance())->GetExperience();

    uint8 CurrentLevel = Cast<UWPGameInstance>(GetGameInstance())->GetSkillLevel(GetFName());

    uint8 MaxLevel = SkillDataAsset->NeedExpForLevelUP.Num();

    if (CurrentLevel >= MaxLevel)
        return;

    uint32 NeedExperience = SkillDataAsset->NeedExpForLevelUP[CurrentLevel];

    if (CurrentExperience >= NeedExperience)
    {
        Cast<UWPGameInstance>(GetGameInstance())->SetSkillLevel(GetFName(), CurrentLevel + 1);
        Cast<UWPGameInstance>(GetGameInstance())->SetExperience(CurrentExperience - NeedExperience);

        Cast<UTextBlock>(GetWidgetFromName("CurrentLevel"))->SetText(FText::FromString(FString::FromInt(CurrentLevel+1)));

        if (CurrentLevel+1 < MaxLevel)
            Cast<UTextBlock>(GetWidgetFromName("Cost"))->SetText(FText::FromString(FString::FromInt(SkillDataAsset->NeedExpForLevelUP[CurrentLevel+1])));
        else
            Cast<UTextBlock>(GetWidgetFromName("Cost"))->SetText(FText::FromString(TEXT("MAX")));

        OnLevelUp.Broadcast();
    }

}
