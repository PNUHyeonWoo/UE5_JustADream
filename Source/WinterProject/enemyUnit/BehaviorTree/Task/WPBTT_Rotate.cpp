// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_Rotate.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTT_Rotate::UWPBTT_Rotate()
{
	NodeName = TEXT("Rotate");

	// TickTask를 사용하기 위해 bNotifyTick을 true로 설정한다.
	bNotifyTick = true;
}

EBTNodeResult::Type UWPBTT_Rotate::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UWPBTT_Rotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = Cast<AWPBaseEnemyUnitAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	EnemyUnit = Cast<AWPBaseEnemyUnit>(AIController->GetCharacter());
	if (EnemyUnit == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (BlackboardComponent == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Blackboard의 NextRotation을 가져온다.
	TargetRotation = BlackboardComponent->GetValueAsRotator(GetSelectedBlackboardKey());

	return EBTNodeResult::InProgress;
}

void UWPBTT_Rotate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	FRotator CurrentRotation = EnemyUnit->GetActorRotation();

	if (FMath::IsNearlyEqual(FRotator::NormalizeAxis(CurrentRotation.Yaw), FRotator::NormalizeAxis(TargetRotation.Yaw), 1.0f))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	EnemyUnit->SetActorRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 4.0f));
}