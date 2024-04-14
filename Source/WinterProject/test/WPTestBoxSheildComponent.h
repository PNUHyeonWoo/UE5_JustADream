// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "interface/WPAttackTargetInterface.h"
#include "WPTestBoxSheildComponent.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPTestBoxSheildComponent : public UBoxComponent, public IWPAttackTargetInterface
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition) override;
	virtual void Die() override;
	
};
