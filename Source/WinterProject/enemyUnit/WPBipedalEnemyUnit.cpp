// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/WPBipedalEnemyUnit.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "AnimInstance/WPBipedalAnimInstance.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"

AWPBipedalEnemyUnit::AWPBipedalEnemyUnit()
{

}

void AWPBipedalEnemyUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AWPBipedalEnemyUnit::BeginPlay()
{
	Super::BeginPlay();

	//// 애님 인스턴스를 가져온다. 가져와서 IdleUppderBodyAnimClip과 같은 변수를 초기화한다.
	//UWPBipedalAnimInstance* AnimInstance = Cast<UWPBipedalAnimInstance>(GetMesh()->GetAnimInstance());

	//if (AnimInstance == nullptr) {
	//	UE_LOG(LogTemp, Error, TEXT("AnimInstance is nullptr"));
	//	return;
	//}

	//AnimInstance->IdleUpperBodyAnimClip = IdleUpperBodyAnimClip;
	//AnimInstance->AlertUpperBodyAnimClip = AlertUpperBodyAnimClip;
	//AnimInstance->AttackAnimClips = AttackAnimClips;

}

void AWPBipedalEnemyUnit::AttackStart()
{
	UWPBipedalAnimInstance* AnimInstance = Cast<UWPBipedalAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->bIsAttacking = true;
	}

}

void AWPBipedalEnemyUnit::AttackEnd()
{
	// 애니메이션 블루프린트 내에서 bIsAttacking이 false가 되고, AttackEnd()가 호출된다.
	// 이 폰이 소지하고 있는 AIController를 가져온다.
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AIController is nullptr"));
		return;
	}

	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(AIController->BrainComponent);
	AttackTaskNode->FinishLatentTask(*BTComponent, EBTNodeResult::Succeeded);
	AttackTaskNode = nullptr;
}