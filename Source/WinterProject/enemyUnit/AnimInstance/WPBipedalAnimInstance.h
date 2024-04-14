// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WPBipedalAnimInstance.generated.h"

class AWPBipedalEnemyUnit;
class AAIController;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBipedalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UWPBipedalAnimInstance();

	virtual void NativeBeginPlay() override;

	virtual void NativeInitializeAnimation() override;

	virtual void PostInitProperties() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_OnAttackEnd();

	UFUNCTION()
	void AnimNotify_OnDealDamageShapecast();

	UFUNCTION()
	void AnimNotify_OnDealDamageRaycast();

	UFUNCTION()
	void AnimNotify_OnSpellCast();

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking;

	UPROPERTY(BlueprintReadWrite)
	float AlertnessLevel;

	UPROPERTY(BlueprintReadWrite)
	UAnimSequence* IdleUpperBodyAnimClip;

	UPROPERTY(BlueprintReadWrite)
	UAnimSequence* AlertUpperBodyAnimClip;

	UPROPERTY(BlueprintReadWrite)
	int AttackToPerform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RelativeMovingAngle;

	UPROPERTY(BlueprintReadWrite)
	TArray<UAnimSequence*> AttackAnimClips;

	UPROPERTY(BlueprintReadWrite)
	AWPBipedalEnemyUnit* EnemyUnit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 IsDead : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRunning;

	UPROPERTY(VisibleAnywhere)
	AAIController* AIController;

	UPROPERTY(VisibleAnywhere)
	UBlackboardComponent* BlackboardComponent;
};
