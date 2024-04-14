// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "dataAsset/statusEffect/WPStatusEffectDataAsset.h"
#include "WPDefenceBuffStatusEffect.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPDefenceBuffStatusEffect : public UWPStatusEffectDataAsset
{
	GENERATED_BODY()
public:
	virtual float TickStatusEffect(class AWPPlayerActor* Player, float Amount, float Resistance, float DeltaTime) override;

protected:
	UPROPERTY(EditAnyWhere)
	float Defence;
	
};
