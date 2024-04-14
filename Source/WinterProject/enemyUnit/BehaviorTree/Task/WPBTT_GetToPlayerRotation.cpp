// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_GetToPlayerRotation.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UWPBTT_GetToPlayerRotation::UWPBTT_GetToPlayerRotation()
{
	NodeName = TEXT("GetToPlayerRotation");
}

EBTNodeResult::Type UWPBTT_GetToPlayerRotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWPBaseEnemyUnitAIController* AIController = Cast<AWPBaseEnemyUnitAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AWPBaseEnemyUnit* EnemyUnit = Cast<AWPBaseEnemyUnit>(AIController->GetPawn());
	if (EnemyUnit == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (BlackboardComponent == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 예상한 플레이어의 위치를 바라보는 FRotator를 구한다.


	FVector PredictedPlayerLocation = BlackboardComponent->GetValueAsVector(FName(TEXT("PredictedPlayerLocation")));
	FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	FVector EnemyLocation = EnemyUnit->GetActorLocation();

	FRotator PlayerRotation = FRotationMatrix::MakeFromX(PlayerLocation - EnemyLocation).Rotator();

	BlackboardComponent->SetValueAsRotator(GetSelectedBlackboardKey(), PlayerRotation);


	return EBTNodeResult::Succeeded;
}