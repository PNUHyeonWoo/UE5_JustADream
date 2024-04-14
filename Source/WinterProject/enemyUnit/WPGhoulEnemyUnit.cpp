// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/WPGhoulEnemyUnit.h"
#include "enemyUnit/AnimInstance/WPGhoulAnimInstance.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BTTaskNode.h"

AWPGhoulEnemyUnit::AWPGhoulEnemyUnit()
{

	UnableAttackTask = EUnableAttackTask::Charge;
	CanDeadShield = 0;
}

void AWPGhoulEnemyUnit::AttackStart() {
	UWPGhoulAnimInstance* AnimInstance = Cast<UWPGhoulAnimInstance>(GetMesh()->GetAnimInstance());

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector PlayerToEnemy = (GetActorLocation() - PlayerLocation).GetSafeNormal();
	FVector DashTarget = PlayerLocation + PlayerToEnemy * 10.f;

	FVector DashDirection = DashTarget - GetActorLocation();
	DashDirection.Normalize();

	DashDirection.Z = 0.1f;

	LaunchCharacter(DashDirection * 1000.f, true, true);

	if (AnimInstance)
	{
		// 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("Ghoul::AttackStart() Call"));
		AnimInstance->bIsAttacking = true;
	}

}

void AWPGhoulEnemyUnit::AttackEnd() {
	AWPBaseEnemyUnitAIController* AIController = Cast<AWPBaseEnemyUnitAIController>(GetController());
	if (AIController == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AIController is nullptr"));
		return;
	}

	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(AIController->BrainComponent);
	AttackTaskNode->FinishLatentTask(*BTComponent, EBTNodeResult::Succeeded);
	//AttackTaskNode = nullptr;
	// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd() Call"));
}