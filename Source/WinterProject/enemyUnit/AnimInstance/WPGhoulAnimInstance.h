// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WPGhoulAnimInstance.generated.h"

class AWPGhoulEnemyUnit;
class AAIController;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPGhoulAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UWPGhoulAnimInstance();

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_OnAttackEnd();

	UFUNCTION()
	void AnimNotify_DashAttack();

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RelativeMovingAngle;

	UPROPERTY(BlueprintReadWrite)
	AWPGhoulEnemyUnit* EnemyUnit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 IsDead : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRunning;

	UPROPERTY()
	AAIController* AIController;
	
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
};
