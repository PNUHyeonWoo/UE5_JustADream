// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enemyUnit/WPBipedalEnemyUnit.h"
#include "WPPillarArcher.generated.h"

class AWPPillar;

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API AWPPillarArcher : public AWPBipedalEnemyUnit
{
	GENERATED_BODY()
	

public:
	AWPPillarArcher();

	virtual void AttackStart() override;
	virtual void AttackEnd() override;

	TArray<AWPPillar*> Pillars;
};
