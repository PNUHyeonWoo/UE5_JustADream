// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "WPBTT_MoveToInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTT_MoveToInAttackRange : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UWPBTT_MoveToInAttackRange();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
