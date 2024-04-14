// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Decorator/WPBTD_Alert.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTD_Alert::UWPBTD_Alert()
{
	NodeName = TEXT("LowAlert");
}

bool UWPBTD_Alert::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AWPBaseEnemyUnit* Enemy = Cast<AWPBaseEnemyUnit>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Enemy == nullptr)	return false;

	if (MinAlertLevel <= Enemy->AlertnessLevel && Enemy->AlertnessLevel <= MaxAlertLevel) {
		return true;
	}
	else {
		return false;
	}
}