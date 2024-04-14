// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "WPBTT_Rotate.generated.h"

class AWPBaseEnemyUnit;
class AWPBaseEnemyUnitAIController;

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTT_Rotate : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UWPBTT_Rotate();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


private:
	UPROPERTY()
	AWPBaseEnemyUnitAIController* AIController;

	UPROPERTY()
	AWPBaseEnemyUnit* EnemyUnit;

	FRotator TargetRotation;
};
