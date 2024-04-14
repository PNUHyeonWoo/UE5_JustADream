// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionTypes.h"
#include "player/WPPlayerActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Components/SplineComponent.h"

AWPBaseEnemyUnitAIController::AWPBaseEnemyUnitAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AWPBaseEnemyUnitAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AWPBaseEnemyUnitAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어
	AWPPlayerActor* Player = Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(Player == nullptr)
	{
		// 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("AIController: Player is nullptr"));
		return;
	}

	// 플레이어와 이 컨트롤러가 소유한 폰과의 거리
	float DistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), Player->GetActorLocation());

	// 거리가 500 이하일 경우 BrainComponent->StartLogic() 호출. 이미 시작되어있으면 호출하지 않음.
	if (DistanceToPlayer <= EnemyPawn->ActiveRange && !BrainComponent->IsRunning()) {
		BrainComponent->StartLogic();
	}
	else if (DistanceToPlayer > EnemyPawn->ActiveRange && BrainComponent->IsRunning()) {
		BrainComponent->StopLogic("Player is too far");
		if (EnemyPawn) {
			MoveToLocation(InitialLocation, 5.f, true, true, true, true, 0, true);
		}
	}
}

void AWPBaseEnemyUnitAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyPawn = Cast<AWPBaseEnemyUnit>(InPawn);

	// 초기 위치 저장
	InitialLocation = EnemyPawn->GetActorLocation();

	if (GetPawn() == nullptr) {
		// 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("AIController: GetPawn is nullptr"));
		return;
	}

	if (PerceptionComponent)
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AWPBaseEnemyUnitAIController::OnTargetPerceptionUpdated);

	if (AIBehavior) {
		RunBehaviorTree(AIBehavior);
	}
}

void AWPBaseEnemyUnitAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	if (Actor == nullptr)
	{
		// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("AIController: Actor is nullptr"));
		return;
	}

	// 시각 감지
	if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
	{
		
		// 만약 Actor가 Player 태그를 가지고 있다면
		if (Actor->ActorHasTag("Player") && !bIsPlayerInSight)
		{
			// BrainComponent가 멈춰있는 상태라면
			if (!BrainComponent->IsRunning()) {
				StopMovement();
			}

			StopMovement();

			bIsPlayerInSight = true;
			// 로그 출력
			UE_LOG(LogTemp, Warning, TEXT("I See Player"));
			GetWorld()->GetTimerManager().ClearTimer(SightTimerHandle);
			GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Player")), Actor);
			EnemyPawn->TimeSinceLastSighted = 0.0f;
			EnemyPawn->AlertnessLevel = 400.0f;
			GetWorld()->GetTimerManager().ClearTimer(DecreaseAlertTimerHandle);
			
		}
		else if (Actor->ActorHasTag("Player") && bIsPlayerInSight) {
			UE_LOG(LogTemp, Warning, TEXT("DecreaseAlert"));
			// 타이머를 사용하여 LastPredictedPlayerLocationTime을 1초마다 1씩 증가시킨다.
			GetWorld()->GetTimerManager().SetTimer(SightTimerHandle, this, &AWPBaseEnemyUnitAIController::SightLostTrack, 1.f, true);
			bIsPlayerInSight = false;
		}

	}

	// 소리 감지
	if (Stimulus.Type == UAISense::GetSenseID(UAISense_Hearing::StaticClass()))
	{
		if (Actor->ActorHasTag("Player")) {
			if (!BrainComponent->IsRunning()) {
				StopMovement();
			}

			// 로그 출력
			UE_LOG(LogTemp, Warning, TEXT("I Hear Player Sound"));
			EnemyPawn->AlertnessLevel = 400.0f;

			GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PredictedPlayerLocation")), Stimulus.StimulusLocation);
		}
	}
}

void AWPBaseEnemyUnitAIController::SightLostTrack() {

	// TimeSinceLastSighted와 SightLostTrackDuration을 출력
	UE_LOG(LogTemp, Warning, TEXT("TimeSinceLastSighted: %f"), EnemyPawn->TimeSinceLastSighted);
	UE_LOG(LogTemp, Warning, TEXT("SightLostTrackDuration: %f"), EnemyPawn->SightLostTrackDuration);
	if (EnemyPawn->TimeSinceLastSighted >= EnemyPawn->SightLostTrackDuration) {
		// 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("Clear Player"));
		GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsPredictedPlayerLocationValid")), false);
		GetBlackboardComponent()->ClearValue(FName(TEXT("Player")));
		// 타이머 종료
		// GetWorld()->GetTimerManager().ClearTimer(SightTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(DecreaseAlertTimerHandle, this, &AWPBaseEnemyUnitAIController::DecreaseAlert, 0.2f, true);
	}
	EnemyPawn->TimeSinceLastSighted++;
}

void AWPBaseEnemyUnitAIController::DecreaseAlert() {
	// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("DecreaseAlert"));
	EnemyPawn->AlertnessLevel -= 1.0f;
	if (EnemyPawn->AlertnessLevel <= 0.0f) {
		EnemyPawn->AlertnessLevel = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(DecreaseAlertTimerHandle);
	}

}