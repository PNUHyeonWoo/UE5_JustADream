// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "struct/WPUndyingAttackTargetActor.h"
#include "interface/WPInteractionInterface.h"
#include "WPShopActor.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API AWPShopActor : public AWPUndyingAttackTargetActor, public IWPInteractionInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(class AWPPlayerActor* PlayerActor) override;

protected:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<USoundWave> OpenSound;
	
};
