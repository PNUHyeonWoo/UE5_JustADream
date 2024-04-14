// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WPStatusEffectDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPStatusEffectDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	virtual float TickStatusEffect(class AWPPlayerActor* Player, float Amount, float Resistance, float DeltaTime);

	static UWPStatusEffectDataAsset* GetStatusEffectData(FName StatusEffectName);

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> StatusEffectIcon;
	
};
