// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/WPAmmoDataAsset.h"

FPrimaryAssetId UWPAmmoDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("WPAmmoDataAsset", GetFName());
}
