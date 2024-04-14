// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WPGunAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPGunAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 PullBolt : 1;
	
};
