// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_SetBoolValue.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
UWPBTT_SetBoolValue::UWPBTT_SetBoolValue()
{
	NodeName = TEXT("Set Bool Value");
}

EBTNodeResult::Type UWPBTT_SetBoolValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(GetSelectedBlackboardKey(), bValue);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}