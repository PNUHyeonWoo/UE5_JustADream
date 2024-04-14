// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Service/WPBTS_UpdateAlertnessLevel.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTS_UpdateAlertnessLevel::UWPBTS_UpdateAlertnessLevel()
{
	NodeName = TEXT("UpdateBlackboardKey");
}

void UWPBTS_UpdateAlertnessLevel::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AWPBaseEnemyUnitAIController* AIController = Cast<AWPBaseEnemyUnitAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return;
	}

	AWPBaseEnemyUnit* EnemyUnit = Cast<AWPBaseEnemyUnit>(AIController->GetPawn());
	if (EnemyUnit == nullptr)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (BlackboardComponent == nullptr)
	{
		return;
	}

	BlackboardComponent->SetValueAsFloat(GetSelectedBlackboardKey(), EnemyUnit->AlertnessLevel);

}