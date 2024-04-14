// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "WPBipedalEnemyUnit.generated.h"


USTRUCT(BlueprintType)
struct FAttackDamageAndPierce
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Pierce;
};

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API AWPBipedalEnemyUnit : public AWPBaseEnemyUnit
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

public:
	AWPBipedalEnemyUnit();

	virtual void AttackStart() override;

	virtual void AttackEnd() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* IdleUpperBodyAnimClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* AlertUpperBodyAnimClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimSequence*> AttackAnimClips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAttackDamageAndPierce> AttackDamageAndPierce = { {20.f, 0.f} };
};
