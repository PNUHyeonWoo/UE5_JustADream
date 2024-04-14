// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/item/WPItemDataAsset.h"
#include <Engine/AssetManager.h>

FPrimaryAssetId UWPItemDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("WPItemDataAsset", GetFName());
}

float UWPItemDataAsset::UseItem(AActor* PlayerActor, float RemainAmount)
{
	return 0.0f;
}

UWPItemDataAsset* UWPItemDataAsset::GetItemData(FName ItemName)
{
	UAssetManager& Manager = UAssetManager::Get();

	FAssetData AssetData;
	Manager.GetPrimaryAssetData(FPrimaryAssetId("WPItemDataAsset", ItemName), AssetData);

	return Cast<UWPItemDataAsset>(AssetData.GetAsset());
}
