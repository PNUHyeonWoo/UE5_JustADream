// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/WPGunDataAsset.h"
#include <Engine/AssetManager.h>

FPrimaryAssetId UWPGunDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("WPGunDataAsset", GetFName());
}

UWPGunDataAsset* UWPGunDataAsset::GetGunData(FName GunName)
{
    UAssetManager& Manager = UAssetManager::Get();

    FAssetData AssetData;
    Manager.GetPrimaryAssetData(FPrimaryAssetId("WPGunDataAsset", GunName), AssetData);

	return Cast<UWPGunDataAsset>(AssetData.GetAsset());
}
