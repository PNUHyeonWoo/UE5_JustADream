// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_GetRandomRotation.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTT_GetRandomRotation::UWPBTT_GetRandomRotation()
{
	NodeName = TEXT("Get Random Rotation");
}

EBTNodeResult::Type UWPBTT_GetRandomRotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	FRotator RandomRotation = FRotator(0.f, FMath::RandRange(-180.0f, 180.0f), 0.f);
	OwnerComp.GetBlackboardComponent()->SetValueAsRotator(GetSelectedBlackboardKey(), RandomRotation);

	return EBTNodeResult::Succeeded;
}