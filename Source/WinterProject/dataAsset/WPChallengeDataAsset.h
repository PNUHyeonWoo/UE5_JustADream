// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WPChallengeDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPChallengeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(EditAnyWhere)
	uint8 maxWeight;
	
};
