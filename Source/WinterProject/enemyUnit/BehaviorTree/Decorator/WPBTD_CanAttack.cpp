// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/BehaviorTree/Decorator/WPBTD_CanAttack.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "player/WPPlayerActor.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "enemyUnit/WPShieldComponent.h"

UWPBTD_CanAttack::UWPBTD_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UWPBTD_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	// 플레이어를 가져온다.
	AWPPlayerActor* Player = Cast<AWPPlayerActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// 컨트롤러는 AIController로, 폰은 EnemyPawn으로 캐스팅
	AWPBaseEnemyUnitAIController* AIController = Cast<AWPBaseEnemyUnitAIController>(OwnerComp.GetAIOwner());
	AWPBaseEnemyUnit* EnemyPawn = Cast<AWPBaseEnemyUnit>(AIController->GetPawn());
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (AIController == nullptr || EnemyPawn == nullptr || BlackboardComponent == nullptr)
	{
		return false;
	}

	// 공격 메소드마다 갱신시간을 체크하고, 정면으로 레이캐스트를 쏘고 플레이어가 맞는지 확인한다.
	// 중간에 다른 몬스터들이 있더라도 최종적으로 플레이어가 맞을수 있다면 공격한다.
	for (int i=0; i<EnemyPawn->AttackValidationMethods.Num(); i++) {

		FAttackValidationMethod AttackMethod = EnemyPawn->AttackValidationMethods[i];

		// 갱신시간을 충족하지 못하면 continue
		if (EnemyPawn->TimeSinceLastSighted - EnemyPawn->SightLostTrackDuration > AttackMethod.AttackValidationTime) {
			// i와 TimeSinceLastSighted - SightLostTrackDuration > AttackMethod.Get<1>()를 출력
UE_LOG(LogTemp, Warning, TEXT("i: %d, TimeSinceLastSighted - SightLostTrackDuration: %f, AttackMethod.AttackValidationTime: %f"), i, EnemyPawn->TimeSinceLastSighted - EnemyPawn->SightLostTrackDuration, AttackMethod.AttackValidationTime);
			continue;
		}

		// 레이캐스트를 쏘는 공격인가?
		if (AttackMethod.bUseRaycast) {

			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActor(EnemyPawn);
			TArray<FHitResult> HitResults;

			// EnemyPawn에서 정면으로 AttackRange만큼 떨어진 곳까지 DrawDebugLine
			DrawDebugLine(EnemyPawn->GetWorld(), EnemyPawn->GetActorLocation(), EnemyPawn->GetActorLocation() + EnemyPawn->GetActorForwardVector() * EnemyPawn->AttackRange, FColor::Red, false, -1.f, 0, 10.f);

			//EnemyPawn->GetWorld()->SweepMultiByChannel(HitResults, EnemyPawn->GetActorLocation(), Player->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(5.f), CollisionQueryParams);
			EnemyPawn->GetWorld()->LineTraceMultiByChannel(HitResults, EnemyPawn->GetActorLocation(), Player->GetActorLocation(), ECC_GameTraceChannel2, CollisionQueryParams);

			for (const FHitResult& Hit : HitResults) {

				// Hit.GetActor()가 Character가 아니라면 return false
				if (!Hit.GetActor()->IsA(ACharacter::StaticClass())) {
					if (!Hit.GetActor()->IsA(UWPShieldComponent::StaticClass()))
						break;
				}

				AActor* HitActor = Hit.GetActor();

				if (!HitActor) continue;

				if (HitActor->Tags.Contains(FName("Player"))) {

					EnemyPawn->AttackToPerform = i;

					return true;
				}
			}


		}
		else {
			
			// 로그 출력
			UE_LOG(LogTemp, Warning, TEXT("AttackMethod.bUseRaycast is false"));

			// 벽너머 공격이고, 갱신시간을 충족했으므로 공격한다.
			EnemyPawn->AttackToPerform = i;

			return true;

		}

	}

	UE_LOG(LogTemp, Warning, TEXT("No AttackMethod is valid"));

	// 어떠한 공격도 할수 없는 상태
	return false;
}