// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_NextPatrolPoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "generateMap/WPPatrolSpline.h"
#include "Components/SplineComponent.h"

UWPBTT_NextPatrolPoint::UWPBTT_NextPatrolPoint()
{
	NodeName = TEXT("NextPatrolPoint");
}

EBTNodeResult::Type UWPBTT_NextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWPBaseEnemyUnit* EnemyPawn = Cast<AWPBaseEnemyUnit>(OwnerComp.GetAIOwner()->GetPawn());

	// EnemyPawn의 블루프린트에 붙어있는 SplineComponent를 가져온다.
	AWPPatrolSpline* PatrolSpline = EnemyPawn->PatrolSpline;
	if (!PatrolSpline)
	{
		return EBTNodeResult::Failed;
	}

	USplineComponent* SplineComponent = PatrolSpline->SplineComponent;

	// EnemyPawn->PatrolIndex를 사용한다.
	// PatrolIndex가 SplineComponent의 PointCount보다 작으면 PatrolIndex를 1 증가시킨다.
	// 그렇지 않으면 PatrolIndex를 0으로 초기화한다.
	// PatrolSpline의 GetLocationAtSplinePoint를 사용하여 PatrolIndex에 해당하는 위치를 가져온다.
	// 가져온 위치를 OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PatrolPoint)를 사용하여 블랙보드에 저장한다.
	// EBTNodeResult::Succeeded를 반환한다.

	if (EnemyPawn)
	{
		if (EnemyPawn->PatrolIndex < SplineComponent->GetNumberOfSplinePoints() - 1)
		{
			EnemyPawn->PatrolIndex++;
		}
		else
		{
			EnemyPawn->PatrolIndex = 0;
		}

		FVector PatrolPoint = SplineComponent->GetLocationAtSplinePoint(EnemyPawn->PatrolIndex, ESplineCoordinateSpace::World);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PatrolPoint);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;

}