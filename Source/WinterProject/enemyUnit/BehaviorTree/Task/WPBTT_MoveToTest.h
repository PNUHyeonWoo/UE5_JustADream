// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "WPBTT_MoveToTest.generated.h"

class AWPBaseEnemyUnitAIController;
class AWPBaseEnemyUnit;

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTT_MoveToTest : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UWPBTT_MoveToTest();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	AWPBaseEnemyUnitAIController* AIController;

	UPROPERTY(EditAnywhere)
	AWPBaseEnemyUnit* EnemyUnit;

	UPROPERTY(EditAnywhere)
	FVector PredictedPlayerLocation;
};
