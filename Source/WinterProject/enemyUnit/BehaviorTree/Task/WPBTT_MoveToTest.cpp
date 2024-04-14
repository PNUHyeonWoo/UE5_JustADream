// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_MoveToTest.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTT_MoveToTest::UWPBTT_MoveToTest()
{
	NodeName = TEXT("MoveToInAttackRange");

	bNotifyTick = true;
}

EBTNodeResult::Type UWPBTT_MoveToTest::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("AbortTask"));

	Super::AbortTask(OwnerComp, NodeMemory);

	AIController->StopMovement();
	return EBTNodeResult::Aborted;
}

void UWPBTT_MoveToTest::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	float distance = FVector::Distance(EnemyUnit->GetActorLocation(), PredictedPlayerLocation);
	if ( distance <= EnemyUnit->AttackRange + 1.f)
	{
		// 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("Succeeded"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 해당 범위 안에 들어오지 않은 경우 공격 범위까지 이동
	AIController->MoveToLocation(PredictedPlayerLocation, EnemyUnit->AttackRange);
}

EBTNodeResult::Type UWPBTT_MoveToTest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AIController = Cast<AWPBaseEnemyUnitAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	EnemyUnit = AIController->GetPawn<AWPBaseEnemyUnit>();
	if (EnemyUnit == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName(TEXT("IsPredictedPlayerLocationValid"))))
	{
		return EBTNodeResult::Failed;
	}

	PredictedPlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("PredictedPlayerLocation")));

	// PredictedPlayerLocation 출력
	UE_LOG(LogTemp, Warning, TEXT("PredictedPlayerLocation : %s"), *PredictedPlayerLocation.ToString());

	// 이미 해당 범위 안에 들어와있는 경우
	double distance = FVector::Distance(EnemyUnit->GetActorLocation(), PredictedPlayerLocation);
	if (distance <= EnemyUnit->AttackRange)
	{
		return EBTNodeResult::Succeeded;
	}

	EPathFollowingRequestResult::Type test = AIController->MoveToLocation(PredictedPlayerLocation, EnemyUnit->AttackRange);

	// test 출력
	UE_LOG(LogTemp, Warning, TEXT("test : %d"), test);

	return EBTNodeResult::InProgress;
}