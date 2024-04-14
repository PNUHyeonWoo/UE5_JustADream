// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "WPBTService_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTService_IsInAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UWPBTService_IsInAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
