// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WPSkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPSkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<UTexture2D> SkillIcon;

	UPROPERTY(EditAnyWhere)
	TArray<uint32> NeedExpForLevelUP;
	
};
