// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_GetRandomLocation.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

// 레벨 바운드
#include "Engine/LevelBounds.h"

UWPBTT_GetRandomLocation::UWPBTT_GetRandomLocation()
{
	NodeName = TEXT("Get Random Location");
}

EBTNodeResult::Type UWPBTT_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWPBaseEnemyUnitAIController* AIController = Cast<AWPBaseEnemyUnitAIController>(OwnerComp.GetAIOwner());
	AWPBaseEnemyUnit* EnemyPawn = Cast<AWPBaseEnemyUnit>(AIController->GetPawn());
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ALevelBounds* LevelBoundsActor = EnemyPawn->GetLevel()->LevelBoundsActor.Get();
	FBox Bounds = LevelBoundsActor->GetComponentsBoundingBox(true);

	FVector Origin = EnemyPawn->GetActorLocation();
	FNavLocation NavLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, RandomMoveRadius, NavLocation))
	{
		if(Bounds.IsInside(NavLocation.Location)) {
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
