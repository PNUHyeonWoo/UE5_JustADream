// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enemyUnit/WPBipedalEnemyUnit.h"
#include "WPWarriorEnemyUnit.generated.h"

class UWPBipedalAnimInstance;

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API AWPWarriorEnemyUnit : public AWPBipedalEnemyUnit
{
	GENERATED_BODY()
	
public:
	AWPWarriorEnemyUnit();

	virtual void AttackStart() override;
	virtual void AttackEnd() override;

};
