// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/statusEffect/WPStatusEffectDataAsset.h"
#include <Engine/AssetManager.h>

FPrimaryAssetId UWPStatusEffectDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("WPStatusEffectDataAsset",GetFName());
}

float UWPStatusEffectDataAsset::TickStatusEffect(AWPPlayerActor* Player, float Amount, float Resistance, float DeltaTime)
{
	return 0.0f;
}

UWPStatusEffectDataAsset* UWPStatusEffectDataAsset::GetStatusEffectData(FName StatusEffectName)
{
	UAssetManager& Manager = UAssetManager::Get();

	FAssetData AssetData;
	Manager.GetPrimaryAssetData(FPrimaryAssetId("WPStatusEffectDataAsset", StatusEffectName), AssetData);

	return Cast<UWPStatusEffectDataAsset>(AssetData.GetAsset());
}
