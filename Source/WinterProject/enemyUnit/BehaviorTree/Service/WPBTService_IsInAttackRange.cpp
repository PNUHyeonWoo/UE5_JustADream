// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Service/WPBTService_IsInAttackRange.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTService_IsInAttackRange::UWPBTService_IsInAttackRange()
{
	NodeName = TEXT("InAttackRangeCheck");
}

void UWPBTService_IsInAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return;
	}

	AWPBaseEnemyUnit* EnemyPawn = Cast<AWPBaseEnemyUnit>(AIController->GetPawn());
	if (EnemyPawn == nullptr)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (BlackboardComponent == nullptr)
	{
		return;
	}

	bool IsPredictedPlayerLocationValid = BlackboardComponent->GetValueAsBool(FName(TEXT("IsPredictedPlayerLocationValid")));
	if (!IsPredictedPlayerLocationValid) return;

	FVector PredictedPlayerLocation = BlackboardComponent->GetValueAsVector(FName(TEXT("PredictedPlayerLocation")));
	FVector PawnLocation = EnemyPawn->GetActorLocation();

	const float Distance = FVector::Distance(FVector(PredictedPlayerLocation.X, PredictedPlayerLocation.Y, 0.f), FVector(PawnLocation.X, PawnLocation.Y, 0.f));
	const float AttackRange = EnemyPawn->AttackRange;

	BlackboardComponent->SetValueAsBool(GetSelectedBlackboardKey(), Distance <= AttackRange);
}