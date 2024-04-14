// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_GetBehindPillarLocation.h"
#include "enemyUnit/WPPillarArcherAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "generateMap/WPPillar.h"

UWPBTT_GetBehindPillarLocation::UWPBTT_GetBehindPillarLocation()
{
	NodeName = TEXT("Get Behind Pillar Location");
}

EBTNodeResult::Type UWPBTT_GetBehindPillarLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller
	AWPPillarArcherAIController* AIController = Cast<AWPPillarArcherAIController>(OwnerComp.GetAIOwner());

	// Get the EnemyPawn
	AWPBaseEnemyUnit* EnemyPawn = Cast<AWPBaseEnemyUnit>(AIController->GetPawn());

	// Get the blackboard component
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();

	// Get the Pillar
	AWPPillar* Pillar = Cast<AWPPillar>(BlackboardComponent->GetValueAsObject("Pillar"));

	if (Pillar == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Get the Pillar Location
	FVector PillarLocation = Pillar->GetActorLocation();

	// Get the Player
	AActor* Player = Cast<AActor>(BlackboardComponent->GetValueAsObject("Player"));

	// Get the Player Location
	FVector PlayerLocation = Player->GetActorLocation();
	
	if (bRandomSign) {
		Angle = FMath::RandRange(0, 1) == 0 ? Angle : -Angle;
	}

	// 플레이어-기둥, 기둥-EnemyPawn의 각도가 원하는 Angle 변수만큼이 되며, 기둥과의 거리가 Pillar->Radius인 위치를 구한다.
	FVector PillarToPlayer = PlayerLocation - PillarLocation;
	FVector PillarToEnemy = EnemyPawn->GetActorLocation() - PillarLocation;
	FVector BehindLocation = PillarLocation + PillarToPlayer.RotateAngleAxis(Angle, FVector(0, 0, 1)).GetSafeNormal() * Pillar->Radius;

	// Set the Blackboard
	BlackboardComponent->SetValueAsVector(GetSelectedBlackboardKey(), BehindLocation);

	return EBTNodeResult::Succeeded;
}