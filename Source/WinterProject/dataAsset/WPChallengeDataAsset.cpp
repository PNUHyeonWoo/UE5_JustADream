// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/WPChallengeDataAsset.h"

FPrimaryAssetId UWPChallengeDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("WPChallengeDataAsset",GetFName());
}
