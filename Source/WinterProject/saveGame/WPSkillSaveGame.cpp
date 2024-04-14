// Fill out your copyright notice in the Description page of Project Settings.


#include "saveGame/WPSkillSaveGame.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/AssetManager.h>

void UWPSkillSaveGame::Integrity()
{
    UAssetManager& Manager = UAssetManager::Get();

    TArray<FAssetData> SkillAssets;
    Manager.GetPrimaryAssetDataList(TEXT("WPSkillDataAsset"), SkillAssets);

    for (FAssetData AssetData : SkillAssets)
        if(!SkillLevels.Find(AssetData.AssetName))
            SkillLevels.Add(AssetData.AssetName, 0);


}
