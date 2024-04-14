// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Task/WPBTT_GetNextPillar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPPillarArcher.h"
#include "generateMap/WPPillar.h"
#include "generateMap/WPArenaMapGenerator.h"
#include "Kismet/GameplayStatics.h"

UWPBTT_GetNextPillar::UWPBTT_GetNextPillar()
{
	NodeName = TEXT("Get Next Pillar");
}

EBTNodeResult::Type UWPBTT_GetNextPillar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Get the blackboard component
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (BlackboardComponent == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Get the current enemy pawn
	AWPPillarArcher* EnemyPawn = Cast<AWPPillarArcher>(AIController->GetPawn());
	if (EnemyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Player = Cast<AActor>(BlackboardComponent->GetValueAsObject("Player"));

	AWPArenaMapGenerator* ArenaMapGenerator = Cast<AWPArenaMapGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), AWPArenaMapGenerator::StaticClass()));
	if (ArenaMapGenerator == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 모든 기둥들 배열
	TArray<AWPPillar*>& Pillars = EnemyPawn->Pillars;

	// 이동 전 기둥
	AWPPillar* CurrentPillar = Cast<AWPPillar>(BlackboardComponent->GetValueAsObject("Pillar"));
	if (CurrentPillar == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 이동 후 기둥
	AWPPillar* NextPillar = nullptr;

	// 현재 기둥을 제외한 모든 기둥에 대해 각도를 기둥별로 계산하고,
	// 각도에 반비례한 확률로 기둥을 선택한다.
	float TotalAngle = 0.0f;
	TArray<float> Angles;
	for (AWPPillar* Pillar : Pillars)
	{

		if (Pillar == CurrentPillar)
		{
			// Angles에서 범위 밖 오류가 발생하지 않도록 -360을 추가
			Angles.Add(-360.f);
			continue;
		}

		FVector CurrentToNext = Pillar->GetActorLocation() - CurrentPillar->GetActorLocation();
		FVector CurrentToPlayer = Player->GetActorLocation() - CurrentPillar->GetActorLocation();
		float Angle = FMath::Acos(FVector::DotProduct(CurrentToNext, CurrentToPlayer) / (CurrentToNext.Size() * CurrentToPlayer.Size())) * 180.0f / PI;
		TotalAngle += Angle;
		Angles.Add(Angle);
	}

	// 각도에 반비례한 확률로 기둥을 선택한다.
	float RandomAngle = FMath::FRandRange(0.0f, TotalAngle);
	float AngleSum = 0.0f;
	for (int32 i = 0; i < Pillars.Num(); i++)
	{
		if (Pillars[i] == CurrentPillar)
		{
			continue;
		}

		AngleSum += Angles[i];
		if (RandomAngle <= AngleSum)
		{
			NextPillar = Pillars[i];
			break;
		}
	}

	// 기둥을 선택하지 못했다면 실패
	if (NextPillar == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Set the next pillar to the blackboard
	BlackboardComponent->SetValueAsObject(GetSelectedBlackboardKey(), NextPillar);

	return EBTNodeResult::Succeeded;
}