// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "WPBTD_CanAttack.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTD_CanAttack : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UWPBTD_CanAttack();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
