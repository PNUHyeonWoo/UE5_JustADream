// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Service/WPBTS_GetPredictedPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UWPBTS_GetPredictedPlayerLocation::UWPBTS_GetPredictedPlayerLocation()
{
	NodeName = TEXT("Get Predicted Player Location");
}

// 가장 최근의 플레이어 위치를 PredictedPlayerLocation 키에 저장한다.
void UWPBTS_GetPredictedPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	AActor* Player = Cast<AActor>(BlackboardComponent->GetValueAsObject(FName(TEXT("Player"))));
	if (Player != nullptr)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsPredictedPlayerLocationValid")), true);
	}
}