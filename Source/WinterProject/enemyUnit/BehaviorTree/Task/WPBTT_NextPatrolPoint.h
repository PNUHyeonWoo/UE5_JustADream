// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "WPBTT_NextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTT_NextPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UWPBTT_NextPatrolPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
