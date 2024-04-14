// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Service/WPBTS_UpdateTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"

UWPBTS_UpdateTask::UWPBTS_UpdateTask()
{
	NodeName = TEXT("UpdateEnum");
}

void UWPBTS_UpdateTask::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	// TaskType 변수의 종류에 따라 Blackboard의 TaskType을 업데이트한다.
	switch (TaskType)
	{
		case ETaskType::Idle:
			BlackboardComponent->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<int32>(EnemyUnit->IdleTask));
			break;
		case ETaskType::Alert:
			BlackboardComponent->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<int32>(EnemyUnit->AlertTask));
			break;
		case ETaskType::UnableAttack:
			BlackboardComponent->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<int32>(EnemyUnit->UnableAttackTask));
			break;
		default:
			break;
	}
}