// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WPAmmoDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPAmmoDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<UTexture2D> AmmoImage;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<UTexture2D> EmptyImage;

	UPROPERTY(EditAnyWhere)
	float ImageXSize;

	UPROPERTY(EditAnyWhere)
	float Damage;

	UPROPERTY(EditAnyWhere)
	int8 penetration;

	UPROPERTY(EditAnyWhere)
	uint8 NumOfWarheads;

	UPROPERTY(EditAnyWhere)
	uint8 isMagazine;
	
};
