// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/WPPillarArcherAIController.h"
#include "enemyUnit/WPPillarArcher.h"
#include "generateMap/WPArenaMapGenerator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "generateMap/WPPillar.h"
#include "BehaviorTree/BehaviorTree.h"

AWPPillarArcherAIController::AWPPillarArcherAIController()
{
}

void AWPPillarArcherAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWPPillarArcherAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AWPPillarArcherAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ArenaMapGenerator = Cast<AWPArenaMapGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), AWPArenaMapGenerator::StaticClass()));

	if (ArenaMapGenerator) {
		ArenaMapGenerator->Fuc_OnAllLevelIsShown.AddUFunction(this, FName("FindNearestPillar"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ArenaMapGenerator Is Invalid"));
	}

	EnemyPawn = Cast<AWPPillarArcher>(GetPawn());

	if (AIBehavior) {

		RunBehaviorTree(AIBehavior);

		// GetBlackboardComponent()가 valid하지 않다면 로그 출력후 리턴
		if (!GetBlackboardComponent()) {
			UE_LOG(LogTemp, Error, TEXT("BlackboardComponent is nullptr"));
			return;
		}

		GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Player")), GetWorld()->GetFirstPlayerController()->GetPawn());
		GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PredictedPlayerLocation")), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsPredictedPlayerLocationValid")), true);
	}

}

void AWPPillarArcherAIController::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

}

void AWPPillarArcherAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AWPPillarArcherAIController::FindNearestPillar() {

	// 가장 가까운 Pillar를 찾아서 Blackboard에 저장
	float NearestDistance = FLT_MAX;
	AWPPillar* NearestPillar = nullptr;

	for (AWPPillar* Pillar : Cast<AWPPillarArcher>(GetPawn())->Pillars) {
		float Distance = FVector::Dist(Pillar->GetActorLocation(), EnemyPawn->GetActorLocation());
		if (Distance < NearestDistance) {
			NearestDistance = Distance;
			NearestPillar = Pillar;
		}
	}

	if (NearestPillar) {
		GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Pillar")), NearestPillar);
	}
	
}