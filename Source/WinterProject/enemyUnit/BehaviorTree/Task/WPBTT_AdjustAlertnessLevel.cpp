// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_AdjustAlertnessLevel.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTT_AdjustAlertnessLevel::UWPBTT_AdjustAlertnessLevel()
{
	NodeName = TEXT("Adjust Alertness Level");
}

EBTNodeResult::Type UWPBTT_AdjustAlertnessLevel::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller
	AWPBaseEnemyUnitAIController* AIController = Cast<AWPBaseEnemyUnitAIController>(OwnerComp.GetAIOwner());

	// Get the EnemyPawn
	AWPBaseEnemyUnit* EnemyPawn = Cast<AWPBaseEnemyUnit>(AIController->GetPawn());

	// Get the blackboard component
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();

	// 키에서 값을 가져온 뒤, AlertnessLevel을 조정한다.
	if (AIController == nullptr) {
		return EBTNodeResult::Failed;
	}

	if (EnemyPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	if (BlackboardComponent == nullptr) {
		return EBTNodeResult::Failed;
	}

	float CurAlertnessLevel;
	// Addition은 덧셈, Substitution은 대입. 범위는 0~500
	switch (AdjustType)
	{
		case EAdjustType::Addition:
			CurAlertnessLevel = BlackboardComponent->GetValueAsFloat(GetSelectedBlackboardKey());
			BlackboardComponent->SetValueAsFloat(GetSelectedBlackboardKey(), FMath::Clamp(CurAlertnessLevel + AdjustValue, 0, 500));
			EnemyPawn->AlertnessLevel = FMath::Clamp(CurAlertnessLevel + AdjustValue, 0, 500);
			break;
		case EAdjustType::Substitution:
			BlackboardComponent->SetValueAsFloat(GetSelectedBlackboardKey(), FMath::Clamp(AdjustValue, 0, 500));
			EnemyPawn->AlertnessLevel = FMath::Clamp(AdjustValue, 0, 500);
			break;
	}

	return EBTNodeResult::Succeeded;
}