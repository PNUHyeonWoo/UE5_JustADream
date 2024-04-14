// Fill out your copyright notice in the Description page of Project Settings.


#include "saveGame/WPChallengeSaveGame.h"
#include <Engine/AssetManager.h>


void UWPChallengeSaveGame::Integrity()
{
    UAssetManager& Manager = UAssetManager::Get();

    TArray<FAssetData> ChallengeAssets;
    Manager.GetPrimaryAssetDataList(TEXT("WPChallengeDataAsset"), ChallengeAssets);

    for (FAssetData AssetData : ChallengeAssets)
        if (!ChallengeWeights.Find(AssetData.AssetName))
            ChallengeWeights.Add(AssetData.AssetName, 0);


}
