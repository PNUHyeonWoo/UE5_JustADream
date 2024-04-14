// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/WPSkillDataAsset.h"

FPrimaryAssetId UWPSkillDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("WPSkillDataAsset", GetFName());
}
