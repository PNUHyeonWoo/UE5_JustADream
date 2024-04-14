// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_MoveToInAttackRange.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"

UWPBTT_MoveToInAttackRange::UWPBTT_MoveToInAttackRange()
{
	NodeName = TEXT("MoveTo");
}

EBTNodeResult::Type UWPBTT_MoveToInAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWPBaseEnemyUnitAIController* AIController = Cast<AWPBaseEnemyUnitAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AWPBaseEnemyUnit* EnemyUnit = AIController->GetPawn<AWPBaseEnemyUnit>();
	if (EnemyUnit == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Super::AcceptableRadius = EnemyUnit->AttackRange;
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	return result;
}