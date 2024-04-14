// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Decorator/WPBTS_UpdatebRotateWhenAlert.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTS_UpdatebRotateWhenAlert::UWPBTS_UpdatebRotateWhenAlert()
{
	NodeName = TEXT("Update bRotateWhenAlert");
}

void UWPBTS_UpdatebRotateWhenAlert::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AWPBaseEnemyUnitAIController* AIController = Cast<AWPBaseEnemyUnitAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		AWPBaseEnemyUnit* EnemyPawn = Cast<AWPBaseEnemyUnit>(AIController->GetPawn());
		if (EnemyPawn)
		{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), EnemyPawn->bRotateWhenAlerted);
		}
	}
}