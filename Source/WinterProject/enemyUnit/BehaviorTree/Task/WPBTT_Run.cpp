// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_Run.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "enemyUnit/AnimInstance/WPBipedalAnimInstance.h"
#include "enemyUnit/WPGhoulEnemyUnit.h"
#include "enemyUnit/AnimInstance/WPGhoulAnimInstance.h"


UWPBTT_Run::UWPBTT_Run()
{
	NodeName = TEXT("Run");
}

EBTNodeResult::Type UWPBTT_Run::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWPBaseEnemyUnit* EnemyPawn = Cast<AWPBaseEnemyUnit>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));

	if (EnemyPawn)
	{
		if (IsRunning) {
			EnemyPawn->GetCharacterMovement()->MaxWalkSpeed = EnemyPawn->RunSpeed;

			if (EnemyPawn->IsA(AWPGhoulEnemyUnit::StaticClass())) {
				Cast<UWPGhoulAnimInstance>(EnemyPawn->GetMesh()->GetAnimInstance())->bIsRunning = true;
			}
			else {
				Cast<UWPBipedalAnimInstance>(EnemyPawn->GetMesh()->GetAnimInstance())->bIsRunning = true;
			}

		}	
		else {
			EnemyPawn->GetCharacterMovement()->MaxWalkSpeed = EnemyPawn->WalkSpeed;

			if (EnemyPawn->IsA(AWPGhoulEnemyUnit::StaticClass())) {
				Cast<UWPGhoulAnimInstance>(EnemyPawn->GetMesh()->GetAnimInstance())->bIsRunning = false;
			}
			else {
				Cast<UWPBipedalAnimInstance>(EnemyPawn->GetMesh()->GetAnimInstance())->bIsRunning = false;
			}
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}