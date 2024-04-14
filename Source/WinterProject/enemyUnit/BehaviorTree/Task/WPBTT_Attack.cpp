// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_Attack.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "enemyUnit/AnimInstance/WPBipedalAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTT_Attack::UWPBTT_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UWPBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AWPBaseEnemyUnit* EnemyUnit = Cast<AWPBaseEnemyUnit>(AIController->GetPawn());

	if (EnemyUnit == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	EnemyUnit->AttackTaskNode = this;
	EnemyUnit->AttackStart();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UWPBTT_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Abort Attack"));

	return EBTNodeResult::Aborted;
}

// Finish Latent Task가 실행되었다면 호출
void UWPBTT_Attack::FinishLatentTask(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type TaskResult)
{
	Super::FinishLatentTask(OwnerComp, TaskResult);

	// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Finish Latent Task"));
}
