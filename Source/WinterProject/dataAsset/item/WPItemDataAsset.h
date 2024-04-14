// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WPItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	virtual float UseItem(AActor* PlayerActor, float RemainAmount);

	static UWPItemDataAsset* GetItemData(FName ItemName);

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ItemImage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ItemShopImage;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditAnywhere)
	float InitialCapacity;

	UPROPERTY(EditAnywhere)
	uint32 Price;

	
};
