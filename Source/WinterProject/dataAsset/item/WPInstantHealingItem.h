// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "dataAsset/item/WPItemDataAsset.h"
#include "WPInstantHealingItem.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPInstantHealingItem : public UWPItemDataAsset
{
	GENERATED_BODY()

public:
	virtual float UseItem(AActor* PlayerActor, float RemainAmount) override;

protected:
	UPROPERTY(EditAnyWhere)
	float HealAmount;
	
};
