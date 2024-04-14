// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "WPGhoulEnemyUnit.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API AWPGhoulEnemyUnit : public AWPBaseEnemyUnit
{
	GENERATED_BODY()
	
public:
	AWPGhoulEnemyUnit();

	virtual void AttackStart() override;
	virtual void AttackEnd() override;
	
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	float Pierce = 1.f;
};
