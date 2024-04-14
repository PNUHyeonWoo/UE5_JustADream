// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WPSwordMasterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPSwordMasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	

protected:
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	UPROPERTY(BlueprintReadOnly)
	float MoveAngle;

	UPROPERTY(BlueprintReadOnly)
	uint8 CanAttack : 1;


public:
	UPROPERTY(BlueprintReadOnly)
	float Parrying;

	UPROPERTY(BlueprintReadWrite)
	uint8 GoToParrying : 1;
};
