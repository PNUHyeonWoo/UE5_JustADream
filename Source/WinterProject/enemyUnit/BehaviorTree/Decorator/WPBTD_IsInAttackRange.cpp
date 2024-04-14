// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Decorator/WPBTD_IsInAttackRange.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTD_IsInAttackRange::UWPBTD_IsInAttackRange()
{
	NodeName = TEXT("Is In Attack Range");
}

bool UWPBTD_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
}